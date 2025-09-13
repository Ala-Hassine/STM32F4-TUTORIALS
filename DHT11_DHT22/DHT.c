#include "DHT.h"

extern TIM_HandleTypeDef htim2;

#define DHT_PORT GPIOA
#define DHT_PIN GPIO_PIN_6

//#define TYPE_DHT11
#define TYPE_DHT22

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM;
uint8_t Presence = 0;

void microDelay(uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  while (__HAL_TIM_GET_COUNTER(&htim2) < delay);
}

void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin 	= GPIO_Pin;
    GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin 	= GPIO_Pin;
    GPIO_InitStruct.Mode 	= GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull 	= GPIO_NOPULL;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void DHT_Start(void)
{
    Set_Pin_Output(DHT_PORT, DHT_PIN);  		// set the pin as output
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, 0);   	// pull the pin low

	#if defined(TYPE_DHT11)
		microDelay(18000);   // >1ms delay (18000us)
	#endif
    #if defined(TYPE_DHT22)
    	microDelay(1300);   // >1ms delay (1300us)
    #endif

    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, 1);   // pull the pin high
    microDelay(30);   // wait for 30us
    Set_Pin_Input(DHT_PORT, DHT_PIN);    // set as input
}

uint8_t DHT_Check_Response(void)
{
    uint8_t Response = 0;
    uint32_t pMillis, cMillis;
    microDelay(40);
    if (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)))
    {
        microDelay(80);
        if ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)))
            Response = 1;
        else
            Response = 0;
    }
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) && (cMillis - pMillis < 2))
    {
        cMillis = HAL_GetTick();
    }
    return Response;
}

uint8_t DHT_Read(void)
{
    uint8_t i = 0;
    uint32_t pMillis, cMillis;
    for (uint8_t j = 0; j < 8; j++)
    {
        // Wait for pin to go high with timeout
        pMillis = HAL_GetTick();
        cMillis = HAL_GetTick();
        while (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) && (cMillis - pMillis < 2))
        {
            cMillis = HAL_GetTick();
        }

        microDelay(40);   // wait for 40 us

        if (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)))   // if the pin is low
        {
            i &= ~(1 << (7 - j));   // write 0
        }
        else
        {
            i |= (1 << (7 - j));  // if the pin is high, write 1
        }
        // Wait for pin to go low with timeout
        pMillis = HAL_GetTick();
        cMillis = HAL_GetTick();
        while ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) && (cMillis - pMillis < 2))
        {
            cMillis = HAL_GetTick();
        }
    }
    return i;
}

void DHT_GetData(DHT_DataTypedef *DHT_Data)
{
    DHT_Start();
    Presence = DHT_Check_Response();
    if (Presence)
    {
        Rh_byte1 	= DHT_Read();
        Rh_byte2 	= DHT_Read();
        Temp_byte1 	= DHT_Read();
        Temp_byte2 	= DHT_Read();
        SUM 		= DHT_Read();
        if (SUM == (Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2))
        {
			#if defined(TYPE_DHT11)
            	// DHT11 data format
				DHT_Data->Humidity = (float)Rh_byte1;
				DHT_Data->Temperature = (float)Temp_byte1;
            #elif defined(TYPE_DHT22)
				if (Temp_byte1 & 0x80) // If MSB is set, temperature is negative
				{
					Temp_byte1 &= 0x7F; // Clear the sign bit
					DHT_Data->Temperature = -((float)((Temp_byte1 << 8) | Temp_byte2) / 10.0);
				}
				else
				{
					DHT_Data->Temperature = (float)((Temp_byte1 << 8) | Temp_byte2) / 10.0;
				}
				DHT_Data->Humidity = (float)((Rh_byte1 << 8) | Rh_byte2) / 10.0;
            #endif
        }
        else
        {
            // Checksum error
            DHT_Data->Temperature = 0;
            DHT_Data->Humidity = 0;
        }
    }
    else
    {
        // No response from sensor
        DHT_Data->Temperature = 0;
        DHT_Data->Humidity = 0;
    }
}
