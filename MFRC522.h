#ifndef MFRC522_H
#define MFRC522_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>

#define PCD_CommandReg     0x01
#define PCD_ComIrqReg      0x04
#define PCD_ErrorReg       0x06
#define PCD_Status2Reg     0x08
#define PCD_FIFODataReg    0x09
#define PCD_FIFOLevelReg   0x0A
#define PCD_BitFramingReg  0x0D
#define PCD_TxControlReg   0x14
#define PCD_TxAutoReg      0x15
#define PCD_RFCfgReg       0x26
#define PCD_TModeReg       0x2A
#define PCD_TPrescalerReg  0x2B
#define PCD_TReloadRegL    0x2C
#define PCD_TReloadRegH    0x2D
#define PCD_DemodReg       0x19
#define PCD_VersionReg     0x37
#define PCD_Idle           0x00
#define PCD_Transceive     0x0C
#define PCD_SoftReset      0x0F
#define PICC_REQA          0x26
#define PICC_SEL_CL1       0x93
#define STATUS_OK          0
#define STATUS_ERROR       1
#define STATUS_TIMEOUT     2

typedef struct 
{
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *csPort;
    uint16_t csPin;
    GPIO_TypeDef *rstPort;
    uint16_t rstPin;
} MFRC522_t;

void    PrintData(const char *format, ...);
void    MFRC522_INIT(MFRC522_t *dev);
void    MFRC522_ANTENNAOFF(MFRC522_t *dev);
void    MFRC522_ANTENNAON(MFRC522_t *dev);
uint8_t MFRC522_READREG(MFRC522_t *dev, uint8_t reg);
void    MFRC522_WRITEREG(MFRC522_t *dev, uint8_t reg, uint8_t value);
void    MFRC522_SETBITMASK(MFRC522_t *dev, uint8_t reg, uint8_t mask);
void    MFRC522_CLEARBITMASK(MFRC522_t *dev, uint8_t reg, uint8_t mask);
uint8_t MFRC522_REQUEST(MFRC522_t *dev, uint8_t *atqa);
uint8_t MFRC522_ANTICOLL(MFRC522_t *dev, uint8_t *uid);
uint8_t MFRC522_READUID(MFRC522_t *dev, uint8_t *uid);
uint8_t MFRC522_WAITCARDREMOVAL(MFRC522_t *dev);
uint8_t MFRC522_WAITCARDDETECT(MFRC522_t *dev);

#endif
