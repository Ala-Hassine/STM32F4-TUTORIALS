#ifndef HC_SR04_H
#define HC_SR04_H

#include "stm32f4xx_hal.h"

#define HC_SR04_USE_TIM1

typedef struct
{
    GPIO_TypeDef* trig_port;
    uint16_t trig_pin;
    GPIO_TypeDef* echo_port;
    uint16_t echo_pin;
} HC_SR04_Config;

void HC_SR04_INIT(HC_SR04_Config* config);
uint16_t HC_SR04_GETDISTANCE(HC_SR04_Config* config);
TIM_HandleTypeDef* HC_SR04_GETTIMERHANDLE(void);

#endif /* HC_SR04_H */
