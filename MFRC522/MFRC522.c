#include "MFRC522.h"

uint8_t atqa[2];

void PrintData(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int len = vsnprintf(uartBuffer, sizeof(uartBuffer), format, args);
    va_end(args);
    if (len > 0)
    {
        HAL_UART_Transmit(&huart2, (uint8_t*)uartBuffer, len, HAL_MAX_DELAY);
    }
}

void MFRC522_INIT(MFRC522_t *dev) 
{
    PrintData("... RFID RC522 Init Started ...");
    // Hardware reset
    HAL_GPIO_WritePin(dev->rstPort, dev->rstPin, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(dev->rstPort, dev->rstPin, GPIO_PIN_SET);
    HAL_Delay(50);
    // Soft reset
    MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_SoftReset);
    HAL_Delay(50);
    // Clear interrupts
    MFRC522_WRITEREG(dev, PCD_ComIrqReg, 0x7F);
    // Flush FIFO
    MFRC522_WRITEREG(dev, PCD_FIFOLevelReg, 0x80);
    // Timer: ~25ms timeout
    MFRC522_WRITEREG(dev, PCD_TModeReg, 0x80);      // Timer starts immediately
    MFRC522_WRITEREG(dev, PCD_TPrescalerReg, 0xA9); // 80kHz clock
    MFRC522_WRITEREG(dev, PCD_TReloadRegH, 0x03);   // 1000 ticks = ~12.5ms
    MFRC522_WRITEREG(dev, PCD_TReloadRegL, 0xE8);
    // RF settings
    MFRC522_WRITEREG(dev, PCD_TxAutoReg, 0x40);     // 100% ASK modulation
    MFRC522_WRITEREG(dev, PCD_RFCfgReg, 0x7F);      // Max gain (48dB)
    MFRC522_WRITEREG(dev, PCD_DemodReg, 0x4D);      // Sensitivity for clones
    // Enable antenna
    MFRC522_ANTENNAON(dev);
    HAL_Delay(10);  // Let RF stabilize
    uint8_t version = MFRC522_READREG(dev, PCD_VersionReg);
    if ((version != 0x91) || (version != 0x92))
    {
    	PrintData("Version : 0x%02X (Counterfeit OK For UID)", version);
    }
    else PrintData("Version : 0x%02X", version);
    uint8_t txCtrl = MFRC522_READREG(dev, PCD_TxControlReg);
    PrintData("TxControlReg : 0x%02X (Expect >= 0x03)", txCtrl);
    PrintData("... RFID RC522 Init Complete ...");
}

void MFRC522_ANTENNAOFF(MFRC522_t *dev) 
{
    MFRC522_CLEARBITMASK(dev, PCD_TxControlReg, 0x03);
    PrintData("... Antenna OFF ...");
}

void MFRC522_ANTENNAON(MFRC522_t *dev) 
{
    MFRC522_SETBITMASK(dev, PCD_TxControlReg, 0x03);
    PrintData("... Antenna ON ...");
}

uint8_t MFRC522_READREG(MFRC522_t *dev, uint8_t reg) 
{
    uint8_t addr = ((reg << 1) & 0x7E) | 0x80;
    uint8_t val = 0;
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(dev->hspi, &addr, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(dev->hspi, &val, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);
    HAL_Delay(1);
    PrintData("ReadReg : 0x%02X -> 0x%02X", reg, val);
    return val;
}

void MFRC522_WRITEREG(MFRC522_t *dev, uint8_t reg, uint8_t value) 
{
    uint8_t addr = (reg << 1) & 0x7E;
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(dev->hspi, &addr, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(dev->hspi, &value, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);
    HAL_Delay(1);
    PrintData("WriteReg : 0x%02X = 0x%02X", reg, value);
}

void MFRC522_SETBITMASK(MFRC522_t *dev, uint8_t reg, uint8_t mask) 
{
    uint8_t tmp = MFRC522_READREG(dev, reg);
    MFRC522_WRITEREG(dev, reg, tmp | mask);
    PrintData("SetBitMask : 0x%02X |= 0x%02X", reg, mask);
}

void MFRC522_CLEARBITMASK(MFRC522_t *dev, uint8_t reg, uint8_t mask) 
{
    uint8_t tmp = MFRC522_READREG(dev, reg);
    MFRC522_WRITEREG(dev, reg, tmp & (~mask));
    PrintData("ClearBitMask : 0x%02X &= ~0x%02X", reg, mask);
}

uint8_t MFRC522_REQUEST(MFRC522_t *dev, uint8_t *atqa) 
{
    PrintData("... Request ...");
    MFRC522_ANTENNAOFF(dev);  // Reset RF
    HAL_Delay(5);  // Allow chip to stabilize
    MFRC522_ANTENNAON(dev);
    HAL_Delay(5);  // Ensure RF is ready
    MFRC522_WRITEREG(dev, PCD_ComIrqReg, 0x7F);      // Clear IRQs
    MFRC522_WRITEREG(dev, PCD_FIFOLevelReg, 0x80);   // Flush FIFO
    MFRC522_WRITEREG(dev, PCD_BitFramingReg, 0x07);  // 7 bits for REQA
    MFRC522_WRITEREG(dev, PCD_FIFODataReg, PICC_REQA);
    HAL_Delay(2);  // Increased for counterfeit chip stability
    MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Transceive);
    MFRC522_SETBITMASK(dev, PCD_BitFramingReg, 0x80);
    // Poll for completion (25ms timeout)
    uint32_t timeout = HAL_GetTick() + 25;
    while (HAL_GetTick() < timeout) 
    {
        uint8_t status2 = MFRC522_READREG(dev, PCD_Status2Reg);
        if (status2 & 0x01) 
        {  
            // Command complete
            uint8_t err = MFRC522_READREG(dev, PCD_ErrorReg);
            if (err & 0x1D) 
            {  
                // Protocol/parity/buffer errors
                PrintData("Request Error : 0x%02X", err);
                MFRC522_ANTENNAOFF(dev);
                HAL_Delay(5);
                MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Idle); // Stop command
                return STATUS_ERROR;
            }
            uint8_t fifoLvl = MFRC522_READREG(dev, PCD_FIFOLevelReg);
            if (fifoLvl >= 2) 
            {  
                // ATQA is 2 bytes
                atqa[0] = MFRC522_READREG(dev, PCD_FIFODataReg);
                atqa[1] = MFRC522_READREG(dev, PCD_FIFODataReg);
                PrintData("Request ATQA : 0x%02X 0x%02X", atqa[0], atqa[1]);
                MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Idle); // Stop command
                HAL_Delay(2);  // Post-command delay
                return STATUS_OK;
            }
            PrintData("Request Bad FIFO Level : %d", fifoLvl);
            MFRC522_ANTENNAOFF(dev);
            HAL_Delay(5);
            MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Idle);
            return STATUS_ERROR;
        }
        HAL_Delay(1);  // Mimic debug log timing
    }
    PrintData("Request Timeout");
    MFRC522_ANTENNAOFF(dev);
    HAL_Delay(5);
    MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Idle);
    return STATUS_TIMEOUT;
}

uint8_t MFRC522_ANTICOLL(MFRC522_t *dev, uint8_t *uid) 
{  
    // Returns 4-byte UID + BCC
    PrintData("... Anticoll ...");
    MFRC522_WRITEREG(dev, PCD_ComIrqReg, 0x7F);      // Clear IRQs
    MFRC522_WRITEREG(dev, PCD_FIFOLevelReg, 0x80);   // Flush FIFO
    MFRC522_WRITEREG(dev, PCD_BitFramingReg, 0x00);  // Full frame
    MFRC522_WRITEREG(dev, PCD_FIFODataReg, PICC_SEL_CL1);  // 0x93
    MFRC522_WRITEREG(dev, PCD_FIFODataReg, 0x20);    // Fixed CRC
    HAL_Delay(2);  // Delay for stability
    MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Transceive);
    MFRC522_SETBITMASK(dev, PCD_BitFramingReg, 0x80);
    uint32_t timeout = HAL_GetTick() + 25;
    while (HAL_GetTick() < timeout) 
    {
        uint8_t status2 = MFRC522_READREG(dev, PCD_Status2Reg);
        if (status2 & 0x01) 
        {  
            // Command complete
            uint8_t err = MFRC522_READREG(dev, PCD_ErrorReg);
            if (err & 0x1D) 
            {
                PrintData("Anticoll Error : 0x%02X", err);
                MFRC522_ANTENNAOFF(dev);
                HAL_Delay(5);
                MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Idle);
                return STATUS_ERROR;
            }
            uint8_t fifoLvl = MFRC522_READREG(dev, PCD_FIFOLevelReg);
            if (fifoLvl == 5) 
            {  
                // 4-byte UID + BCC
                for (int i = 0; i < 5; i++) 
                {
                    uid[i] = MFRC522_READREG(dev, PCD_FIFODataReg);
                }
                // Validate BCC
                uint8_t calcBcc = uid[0] ^ uid[1] ^ uid[2] ^ uid[3];
                if (uid[4] != calcBcc) 
                {
                    PrintData("Anticoll Bad BCC : calc=0x%02X, got=0x%02X", calcBcc, uid[4]);
                    MFRC522_ANTENNAOFF(dev);
                    HAL_Delay(5);
                    MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Idle);
                    return STATUS_ERROR;
                }
                PrintData("Anticoll UID : %02X %02X %02X %02X %02X", uid[0], uid[1], uid[2], uid[3], uid[4]);
                MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Idle);
                HAL_Delay(2);  // Post-command delay
                return STATUS_OK;
            }
            PrintData("Anticoll Bad FIFO Level : %d", fifoLvl);
            MFRC522_ANTENNAOFF(dev);
            HAL_Delay(5);
            MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Idle);
            return STATUS_ERROR;
        }
        HAL_Delay(1);  // Mimic debug log timing
    }
    PrintData("Anticoll Timeout");
    MFRC522_ANTENNAOFF(dev);
    HAL_Delay(5);
    MFRC522_WRITEREG(dev, PCD_CommandReg, PCD_Idle);
    return STATUS_TIMEOUT;
}

uint8_t MFRC522_READUID(MFRC522_t *dev, uint8_t *uid) 
{  
    // Output : uid[4]
    PrintData("... Reading UID ...");
    // Card Detected, Read UID
    uint8_t rawUid[5];
    if (MFRC522_ANTICOLL(dev, rawUid) != STATUS_OK) 
    {
    	PrintData("Anticollision Failed");
        return STATUS_ERROR;
    }
    // Copy UID (drop BCC)
    for (int i = 0; i < 4; i++) 
    {
        uid[i] = rawUid[i];
    }
    PrintData("Card UID : %02X %02X %02X %02X", uid[0], uid[1], uid[2], uid[3]);
    return STATUS_OK;
}

uint8_t MFRC522_WAITCARDREMOVAL(MFRC522_t *dev)
{
    PrintData("... Waiting For Card Removal ...");
    while (1) 
    {
        if (MFRC522_REQUEST(dev, atqa) != STATUS_OK) 
        {
        	PrintData("Card Removed !!!");
            return STATUS_OK; // Card removed, return success
        }
        HAL_Delay(100); // Poll every 100ms to check if card is still present
    }
}

uint8_t MFRC522_WAITCARDDETECT(MFRC522_t *dev)
{
	atqa[0] = atqa[1] = 0;
	PrintData("... Waiting For The Card ...");
	while (1){
	    if (MFRC522_REQUEST(dev, atqa) == STATUS_OK) 
        {
	    	PrintData("Card Detected !!!");
	        return STATUS_OK;
	    }
	    HAL_Delay(100);	// Poll every 100ms to check if card is  present
	}
}
