#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include "stm32f4xx_hal.h"

typedef enum
{
    SEVEN_SEG_COMMON_CATHODE,
    SEVEN_SEG_COMMON_ANODE
} SevenSeg_Type_t;

typedef struct
{
    GPIO_TypeDef* gpio_port;
    uint16_t PIN_A;
    uint16_t PIN_B;
    uint16_t PIN_C;
    uint16_t PIN_D;
    uint16_t PIN_E;
    uint16_t PIN_F;
    uint16_t PIN_G;
    uint16_t PIN_DP;
    SevenSeg_Type_t type;
} SevenSeg_Config_t;

void SEVEN_SEG_INIT(SevenSeg_Config_t* config);
void SEVEN_SEG_DISPLAYDIGIT(uint8_t digit);
void SEVEN_SEG_DISPLAYCHAR(char character);
void SEVEN_SEG_DISPLAYHEX(uint8_t hex_value);
void SEVEN_SEG_CLEAR(void);
void SEVEN_SEG_ENABLEDECIMALPOINT(void);
void SEVEN_SEG_DISABLEDECIMALPOINT(void);
uint8_t SEVEN_SEG_DIGITTOSEGMENTS(uint8_t digit);
uint8_t SEVEN_SEG_CHARTOSEGMENTS(char character);

#endif /* SEVEN_SEGMENT_H */
