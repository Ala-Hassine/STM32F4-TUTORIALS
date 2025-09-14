#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "stm32f4xx_hal.h"

typedef struct
{
    uint16_t x_value;
    uint16_t y_value;
    uint16_t x_center;
    uint16_t y_center;
    uint16_t deadzone;
    char direction[16];
} Joystick_TypeDef;

typedef enum
{
    JOYSTICK_CENTER,
    JOYSTICK_UP,
    JOYSTICK_DOWN,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT,
    JOYSTICK_UP_LEFT,
    JOYSTICK_UP_RIGHT,
    JOYSTICK_DOWN_LEFT,
    JOYSTICK_DOWN_RIGHT
} Joystick_Direction;

void JOYSTICK_INIT(Joystick_TypeDef *joystick, uint16_t deadzone);
void JOYSTICK_READ(Joystick_TypeDef *joystick, ADC_HandleTypeDef *hadc_x, ADC_HandleTypeDef *hadc_y);
Joystick_Direction JOYSTICK_GETDIRECTION(Joystick_TypeDef *joystick);
const char* JOYSTICK_GETDIRECTIONSTRING(Joystick_Direction direction);
void JOYSTICK_CALIBRATECENTER(Joystick_TypeDef *joystick, ADC_HandleTypeDef *hadc_x, ADC_HandleTypeDef *hadc_y);

#endif /* JOYSTICK_H */
