#ifndef DHT_H_
#define DHT_H_

#include "stm32f4xx_hal.h"

typedef struct
{
	float Temperature;
	float Humidity;
} DHT_DataTypedef;

void DHT_GetData (DHT_DataTypedef *DHT_Data);

#endif 
