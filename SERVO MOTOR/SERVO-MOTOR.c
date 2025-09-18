#include "SERVO-MOTOR.h"
#include "math.h"

// Default Servo Parameters (For SG90 Servo)
#define DEFAULT_MIN_PULSE      500     // 0.5ms
#define DEFAULT_MAX_PULSE      2500    // 2.5ms
#define DEFAULT_MIN_ANGLE      0       // 0 degrees
#define DEFAULT_MAX_ANGLE      180     // 180 degrees


Servo_Status SERVO_INIT(Servo_HandleTypeDef *hservo, TIM_HandleTypeDef *htim, uint32_t channel, uint32_t min_pulse, uint32_t max_pulse, uint32_t min_angle, uint32_t max_angle)
{
    if (hservo == NULL || htim == NULL)
    {
        return SERVO_INVALID_PARAM;
    }
    if (min_pulse >= max_pulse || min_angle >= max_angle)
    {
        return SERVO_INVALID_PARAM;
    }
    hservo->htim 			= htim;
    hservo->channel 		= channel;
    hservo->min_pulse 		= min_pulse;
    hservo->max_pulse 		= max_pulse;
    hservo->min_angle 		= min_angle;
    hservo->max_angle 		= max_angle;
    hservo->current_angle 	= min_angle;
    if (HAL_TIM_PWM_Start(hservo->htim, hservo->channel) != HAL_OK)
    {
        return SERVO_ERROR;
    }
    SERVO_SETANGLE(hservo, hservo->min_angle);
    return SERVO_OK;
}

Servo_Status SERVO_SETANGLE(Servo_HandleTypeDef *hservo, uint32_t angle)
{
    if (hservo == NULL)
    {
        return SERVO_INVALID_PARAM;
    }
    if (angle < hservo->min_angle)
    {
        angle = hservo->min_angle;
    }
    else if (angle > hservo->max_angle)
    {
        angle = hservo->max_angle;
    }
    uint32_t microseconds = ANGLETOMICROSECONDS(hservo, angle);
    return SERVO_SETMICROSECONDS(hservo, microseconds);
}

Servo_Status SERVO_SETMICROSECONDS(Servo_HandleTypeDef *hservo, uint32_t microseconds)
{
    if (hservo == NULL)
    {
        return SERVO_INVALID_PARAM;
    }
    if (microseconds < hservo->min_pulse)
    {
        microseconds = hservo->min_pulse;
    }
    else if (microseconds > hservo->max_pulse)
    {
        microseconds = hservo->max_pulse;
    }
    uint32_t compare_value = microseconds;
    switch (hservo->channel)
    {
        case TIM_CHANNEL_1: hservo->htim->Instance->CCR1 = compare_value; break;
        case TIM_CHANNEL_2: hservo->htim->Instance->CCR2 = compare_value; break;
        case TIM_CHANNEL_3: hservo->htim->Instance->CCR3 = compare_value; break;
        case TIM_CHANNEL_4: hservo->htim->Instance->CCR4 = compare_value; break;
        default: return SERVO_ERROR;
    }
    hservo->current_angle = MICROSECONDSTOANGLE(hservo, microseconds);
    return SERVO_OK;
}

uint32_t SERVO_GETCURRENTANGLE(Servo_HandleTypeDef *hservo)
{
    if (hservo == NULL)
    {
        return 0;
    }
    return hservo->current_angle;
}

Servo_Status SERVO_DEINIT(Servo_HandleTypeDef *hservo)
{
    if (hservo == NULL)
    {
        return SERVO_INVALID_PARAM;
    }
    if (HAL_TIM_PWM_Stop(hservo->htim, hservo->channel) != HAL_OK)
    {
        return SERVO_ERROR;
    }
    return SERVO_OK;
}

uint32_t ANGLETOMICROSECONDS(Servo_HandleTypeDef *hservo, uint32_t angle)
{
    if (hservo == NULL)
    {
        return 0;
    }
    return hservo->min_pulse + ((angle - hservo->min_angle) * (hservo->max_pulse - hservo->min_pulse)) / (hservo->max_angle - hservo->min_angle);
}

uint32_t MICROSECONDSTOANGLE(Servo_HandleTypeDef *hservo, uint32_t microseconds)
{
    if (hservo == NULL)
    {
        return 0;
    }
    return hservo->min_angle + ((microseconds - hservo->min_pulse) * (hservo->max_angle - hservo->min_angle)) / (hservo->max_pulse - hservo->min_pulse);
}
