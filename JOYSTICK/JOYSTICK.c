#include "joystick.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

void JOYSTICK_INIT(Joystick_TypeDef *joystick, uint16_t deadzone)
{
    joystick->x_value = 0;
    joystick->y_value = 0;
    joystick->x_center = 2048; // Default center for 12-bit ADC
    joystick->y_center = 2048;
    joystick->deadzone = deadzone;
    strcpy(joystick->direction, "CENTER");
}

void JOYSTICK_READ(Joystick_TypeDef *joystick, ADC_HandleTypeDef *hadc_x, ADC_HandleTypeDef *hadc_y)
{
    // Read X-axis (Example : PA1 - ADC1 Channel 1)
    HAL_ADC_Start(hadc_x);
    HAL_ADC_PollForConversion(hadc_x, 100);
    joystick->x_value = HAL_ADC_GetValue(hadc_x);
    HAL_ADC_Stop(hadc_x);
    // Read Y-axis (Example : PA2 - ADC2 Channel 2)
    HAL_ADC_Start(hadc_y);
    HAL_ADC_PollForConversion(hadc_y, 100);
    joystick->y_value = HAL_ADC_GetValue(hadc_y);
    HAL_ADC_Stop(hadc_y);
}

void JOYSTICK_CALIBRATECENTER(Joystick_TypeDef *joystick, ADC_HandleTypeDef *hadc_x, ADC_HandleTypeDef *hadc_y)
{
    uint32_t sum_x = 0, sum_y = 0;
    const uint8_t samples = 10;
    for (uint8_t i = 0; i < samples; i++)
    {
        JOYSTICK_READ(joystick, hadc_x, hadc_y);
        sum_x += joystick->x_value;
        sum_y += joystick->y_value;
        HAL_Delay(10);
    }
    joystick->x_center = sum_x / samples;
    joystick->y_center = sum_y / samples;
}

Joystick_Direction JOYSTICK_GETDIRECTION(Joystick_TypeDef *joystick)
{
    int16_t x_diff = (int16_t)joystick->x_value - (int16_t)joystick->x_center;
    int16_t y_diff = (int16_t)joystick->y_center - (int16_t)joystick->y_value;
    if (abs(x_diff) < joystick->deadzone && abs(y_diff) < joystick->deadzone)
    {
        return JOYSTICK_CENTER;
    }
    float angle = atan2f(y_diff, x_diff) * 180.0f / M_PI;
    if (angle < 0) angle += 360.0f;
    if (angle >= 337.5f || angle < 22.5f) {
        return JOYSTICK_RIGHT;
    } else if (angle >= 22.5f && angle < 67.5f) {
        return JOYSTICK_UP_RIGHT;
    } else if (angle >= 67.5f && angle < 112.5f) {
        return JOYSTICK_UP;
    } else if (angle >= 112.5f && angle < 157.5f) {
        return JOYSTICK_UP_LEFT;
    } else if (angle >= 157.5f && angle < 202.5f) {
        return JOYSTICK_LEFT;
    } else if (angle >= 202.5f && angle < 247.5f) {
        return JOYSTICK_DOWN_LEFT;
    } else if (angle >= 247.5f && angle < 292.5f) {
        return JOYSTICK_DOWN;
    } else {
        return JOYSTICK_DOWN_RIGHT;
    }
}

const char* JOYSTICK_GETDIRECTIONSTRING(Joystick_Direction direction)
{
    switch (direction)
    {
        case JOYSTICK_CENTER:     return "CENTER";
        case JOYSTICK_UP:         return "UP";
        case JOYSTICK_DOWN:       return "DOWN";
        case JOYSTICK_LEFT:       return "LEFT";
        case JOYSTICK_RIGHT:      return "RIGHT";
        case JOYSTICK_UP_LEFT:    return "UP-LEFT";
        case JOYSTICK_UP_RIGHT:   return "UP-RIGHT";
        case JOYSTICK_DOWN_LEFT:  return "DOWN-LEFT";
        case JOYSTICK_DOWN_RIGHT: return "DOWN-RIGHT";
        default:                  return "UNKNOWN";
    }
}
