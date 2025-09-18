#include "MOTOR_DC.h"
#include "string.h"
#include "math.h"

void MOTOR_INIT(My_Motor_DC* motor, TIM_HandleTypeDef* timer, uint32_t channel, GPIO_TypeDef* in1_port, uint16_t in1_pin, GPIO_TypeDef* in2_port, uint16_t in2_pin)
{
    motor->pwm_timer 			= timer;
    motor->pwm_channel 			= channel;
    motor->in1_port 			= in1_port;
    motor->in1_pin 				= in1_pin;
    motor->in2_port 			= in2_port;
    motor->in2_pin 				= in2_pin;
    motor->current_speed 		= 0;
    motor->current_direction 	= MOTOR_DIRECTION_STOP;
    HAL_TIM_PWM_Start(motor->pwm_timer, motor->pwm_channel);
    MOTOR_STOP(motor);
}

void MOTOR_SETSPEED(My_Motor_DC* motor, uint8_t speed)
{
    if (speed > 255)
    {
        speed = 255;
    }
    motor->current_speed 	= speed;
    uint32_t auto_reload 	= __HAL_TIM_GET_AUTORELOAD(motor->pwm_timer);
    uint32_t compare_value 	= (speed * auto_reload) / 255;
    __HAL_TIM_SET_COMPARE(motor->pwm_timer, motor->pwm_channel, compare_value);
}

void MOTOR_SETDIRECTION(My_Motor_DC* motor, Motor_Direction_t direction)
{
    motor->current_direction = direction;
    switch (direction)
    {
        case MOTOR_DIRECTION_FORWARD:
            HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_RESET);
            break;
        case MOTOR_DIRECTION_BACKWARD:
            HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_SET);
            break;
        case MOTOR_DIRECTION_STOP:
            HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_RESET);
            break;
    }
}

void MOTOR_STOP(My_Motor_DC* motor)
{
	MOTOR_SETSPEED(motor, 0);
    MOTOR_SETDIRECTION(motor, MOTOR_DIRECTION_STOP);
}

void MOTOR_BRAKE(My_Motor_DC* motor)
{
    HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_SET);
    MOTOR_SETSPEED(motor, 255);
    motor->current_direction = MOTOR_DIRECTION_STOP;
}

uint8_t MOTOR_GETSPEED(const My_Motor_DC* motor)
{
    return motor->current_speed;
}

Motor_Direction_t MOTOR_GETDIRECTION(const My_Motor_DC* motor)
{
    return motor->current_direction;
}

void MOTOR_RAMPSPEED(My_Motor_DC* motor, uint8_t target_speed, uint16_t ramp_time_ms)
{
    uint8_t current_speed = motor->current_speed;
    if (current_speed == target_speed)
    {
        return;
    }
    uint16_t num_steps;
    uint8_t step_size = 1;

    if (target_speed > current_speed)
    {
        num_steps = target_speed - current_speed;
    }
    else
    {
        num_steps = current_speed - target_speed;
    }
    if (ramp_time_ms < num_steps)
    {
        step_size = (num_steps + ramp_time_ms - 1) / ramp_time_ms;
        num_steps = (num_steps + step_size - 1) / step_size;
    }
    uint16_t step_delay = (num_steps > 0) ? (ramp_time_ms / num_steps) : 0;
    if (step_delay == 0)
    {
        step_delay = 1;
    }
    if (target_speed > current_speed)
    {
        for (uint16_t i = 1; i <= num_steps; i++)
        {
            uint8_t new_speed = current_speed + (i * step_size);
            if (new_speed > target_speed)
            {
                new_speed = target_speed;
            }
            MOTOR_SETSPEED(motor, new_speed);
            HAL_Delay(step_delay);
        }
    }
    else
    {
        for (uint16_t i = 1; i <= num_steps; i++)
        {
            uint8_t new_speed = current_speed - (i * step_size);
            if (new_speed < target_speed)
            {
                new_speed = target_speed;
            }
            MOTOR_SETSPEED(motor, new_speed);
            HAL_Delay(step_delay);
        }
    }
    MOTOR_SETSPEED(motor, target_speed);
}
