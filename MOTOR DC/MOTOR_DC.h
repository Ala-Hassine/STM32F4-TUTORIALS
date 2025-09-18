#ifndef MOTOR_DC_H
#define MOTOR_DC_H

#include "main.h"
#include "stm32f4xx_hal.h"

typedef enum
{
    MOTOR_DIRECTION_FORWARD = 0,
    MOTOR_DIRECTION_BACKWARD,
    MOTOR_DIRECTION_STOP
} Motor_Direction_t;

typedef struct
{
    TIM_HandleTypeDef* 	pwm_timer;
    uint32_t 			pwm_channel;
    GPIO_TypeDef* 		in1_port;
    uint16_t 			in1_pin;
    GPIO_TypeDef* 		in2_port;
    uint16_t 			in2_pin;
    uint8_t 			current_speed;
    Motor_Direction_t 	current_direction;
} My_Motor_DC;

void MOTOR_INIT(My_Motor_DC* motor, TIM_HandleTypeDef* timer, uint32_t channel, GPIO_TypeDef* in1_port, uint16_t in1_pin, GPIO_TypeDef* in2_port, uint16_t in2_pin);
void MOTOR_SETSPEED(My_Motor_DC* motor, uint8_t speed);
void MOTOR_SETDIRECTION(My_Motor_DC* motor, Motor_Direction_t direction);
void MOTOR_STOP(My_Motor_DC* motor);
void MOTOR_BRAKE(My_Motor_DC* motor);
uint8_t MOTOR_GETSPEED(const My_Motor_DC* motor);
Motor_Direction_t MOTOR_GETDIRECTION(const My_Motor_DC* motor);
void MOTOR_RAMPSPEED(My_Motor_DC* motor, uint8_t target_speed, uint16_t ramp_time_ms);

#endif /* MOTOR_DC_H */
