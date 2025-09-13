#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f4xx_hal.h"

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

#define ROW1_GPIO_PORT 		GPIOA
#define ROW1_PIN 			GPIO_PIN_7
#define ROW2_GPIO_PORT 		GPIOA
#define ROW2_PIN 			GPIO_PIN_6
#define ROW3_GPIO_PORT 		GPIOA
#define ROW3_PIN 			GPIO_PIN_5
#define ROW4_GPIO_PORT 		GPIOA
#define ROW4_PIN 			GPIO_PIN_4

#define COL1_GPIO_PORT 		GPIOA
#define COL1_PIN 			GPIO_PIN_3
#define COL2_GPIO_PORT		GPIOA
#define COL2_PIN 			GPIO_PIN_2
#define COL3_GPIO_PORT 		GPIOA
#define COL3_PIN 			GPIO_PIN_1
#define COL4_GPIO_PORT 		GPIOA
#define COL4_PIN 			GPIO_PIN_0

void KEYPAD_INIT(void);
char KEYPAD_READ(void);
char KEYPAD_SCAN(void);

#endif /* KEYPAD_H */
