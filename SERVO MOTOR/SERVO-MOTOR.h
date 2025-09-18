#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "stm32f4xx_hal.h"

typedef struct
{
    TIM_HandleTypeDef *htim;      // Timer handle
    uint32_t channel;             // Timer channel
    uint32_t min_pulse;           // Minimum pulse width in microseconds
    uint32_t max_pulse;           // Maximum pulse width in microseconds
    uint32_t min_angle;           // Minimum angle in degrees
    uint32_t max_angle;           // Maximum angle in degrees
    uint32_t current_angle;       // Current angle in degrees
} Servo_HandleTypeDef;

typedef enum
{
    SERVO_OK = 0,
    SERVO_ERROR,
    SERVO_INVALID_PARAM
} Servo_Status;

Servo_Status 	SERVO_INIT(Servo_HandleTypeDef *hservo, TIM_HandleTypeDef *htim, uint32_t channel, uint32_t min_pulse, uint32_t max_pulse, uint32_t min_angle, uint32_t max_angle);
Servo_Status 	SERVO_SETANGLE(Servo_HandleTypeDef *hservo, uint32_t angle);
Servo_Status 	SERVO_SETMICROSECONDS(Servo_HandleTypeDef *hservo, uint32_t microseconds);
uint32_t 		SERVO_GETCURRENTANGLE(Servo_HandleTypeDef *hservo);
Servo_Status 	SERVO_DEINIT(Servo_HandleTypeDef *hservo);
uint32_t 		ANGLETOMICROSECONDS(Servo_HandleTypeDef *hservo, uint32_t angle);
uint32_t 		MICROSECONDSTOANGLE(Servo_HandleTypeDef *hservo, uint32_t microseconds);

#endif /* SERVO_MOTOR_H */
