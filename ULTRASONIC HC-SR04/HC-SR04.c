#include "HC-SR04.h"
#include "stdio.h"

static HC_SR04_Config* sensor_config;
static TIM_HandleTypeDef* hc_sr04_timer = NULL;

TIM_HandleTypeDef* HC_SR04_GETTIMERHANDLE(void)
{
    return hc_sr04_timer;
}

void HC_SR04_INIT(HC_SR04_Config* config)
{
    sensor_config = config;
    #ifdef HC_SR04_USE_TIM1
    	extern TIM_HandleTypeDef htim1;
    	hc_sr04_timer = &htim1;
    #elif defined(HC_SR04_USE_TIM2)
    	extern TIM_HandleTypeDef htim2;
    	hc_sr04_timer = &htim2;
    #elif defined(HC_SR04_USE_TIM3)
    	extern TIM_HandleTypeDef htim3;
    	hc_sr04_timer = &htim3;
	#elif defined(HC_SR04_USE_TIM4)
		extern TIM_HandleTypeDef htim4;
		hc_sr04_timer = &htim4;
	#elif defined(HC_SR04_USE_TIM5)
		extern TIM_HandleTypeDef htim5;
		hc_sr04_timer = &htim5;
	#elif defined(HC_SR04_USE_TIM6)
		extern TIM_HandleTypeDef htim6;
		hc_sr04_timer = &htim6;
	#elif defined(HC_SR04_USE_TIM7)
		extern TIM_HandleTypeDef htim7;
		hc_sr04_timer = &htim7;
	#elif defined(HC_SR04_USE_TIM8)
		extern TIM_HandleTypeDef htim8;
		hc_sr04_timer = &htim8;
	#elif defined(HC_SR04_USE_TIM9)
		extern TIM_HandleTypeDef htim9;
		hc_sr04_timer = &htim9;
	#elif defined(HC_SR04_USE_TIM10)
		extern TIM_HandleTypeDef htim10;
		hc_sr04_timer = &htim10;
	#elif defined(HC_SR04_USE_TIM11)
		extern TIM_HandleTypeDef htim11;
		hc_sr04_timer = &htim11;
	#elif defined(HC_SR04_USE_TIM12)
		extern TIM_HandleTypeDef htim12;
		hc_sr04_timer = &htim12;
	#elif defined(HC_SR04_USE_TIM13)
		extern TIM_HandleTypeDef htim13;
		hc_sr04_timer = &htim13;
	#elif defined(HC_SR04_USE_TIM14)
		extern TIM_HandleTypeDef htim14;
		hc_sr04_timer = &htim14;
    #else
    	#error "No Timer Defined For HC-SR04 ... Please Define HC_SR04_USE_TIMx In HC-SR04.h"
    #endif
    HAL_GPIO_WritePin(sensor_config->trig_port, sensor_config->trig_pin, GPIO_PIN_RESET);
    if (hc_sr04_timer != NULL)
    {
        HAL_TIM_Base_Start(hc_sr04_timer);
    }
}

uint16_t HC_SR04_GETDISTANCE(HC_SR04_Config* config)
{
    if (hc_sr04_timer == NULL)
    {
        return 0;
    }
    uint32_t val1 = 0;
    uint32_t val2 = 0;
    uint32_t pMillis;
    uint16_t distance = 0;
    // Send 10μs trigger pulse
    HAL_GPIO_WritePin(config->trig_port, config->trig_pin, GPIO_PIN_SET);
    __HAL_TIM_SET_COUNTER(hc_sr04_timer, 0);
    while (__HAL_TIM_GET_COUNTER(hc_sr04_timer) < 10);  // wait for 10 us
    HAL_GPIO_WritePin(config->trig_port, config->trig_pin, GPIO_PIN_RESET);
    // Wait for echo to go high (with timeout)
    pMillis = HAL_GetTick();
    while (!(HAL_GPIO_ReadPin(config->echo_port, config->echo_pin)) &&
           pMillis + 10 > HAL_GetTick());
    val1 = __HAL_TIM_GET_COUNTER(hc_sr04_timer);
    // Wait for echo to go low (with timeout)
    pMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin(config->echo_port, config->echo_pin)) &&
           pMillis + 50 > HAL_GetTick());
    val2 = __HAL_TIM_GET_COUNTER(hc_sr04_timer);
    distance = (val2 - val1) * 0.034 / 2;
    return distance;
}
