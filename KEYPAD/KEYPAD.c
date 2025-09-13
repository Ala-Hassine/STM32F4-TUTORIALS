#include "KEYPAD.h"

static void SET_ROW(uint8_t row, GPIO_PinState state);
static GPIO_PinState COLUMN_READ(uint8_t col);

void KEYPAD_INIT(void)
{
    HAL_GPIO_WritePin(ROW1_GPIO_PORT, ROW1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW2_GPIO_PORT, ROW2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW3_GPIO_PORT, ROW3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW4_GPIO_PORT, ROW4_PIN, GPIO_PIN_SET);
}

char KEYPAD_READ(void)
{
    char key = Keypad_Scan();
    if (key != 0)
    {
        while (Keypad_Scan() != 0);
    }
    return key;
}

char KEYPAD_SCAN(void)
{
    // Scan Row 1
    SET_ROW(1, GPIO_PIN_RESET);
    SET_ROW(2, GPIO_PIN_SET);
    SET_ROW(3, GPIO_PIN_SET);
    SET_ROW(4, GPIO_PIN_SET);

    HAL_Delay(10);

    if (COLUMN_READ(1) == GPIO_PIN_RESET) return '1';
    if (COLUMN_READ(2) == GPIO_PIN_RESET) return '2';
    if (COLUMN_READ(3) == GPIO_PIN_RESET) return '3';
    if (COLUMN_READ(4) == GPIO_PIN_RESET) return 'A';

    // Scan Row 2
    SET_ROW(1, GPIO_PIN_SET);
    SET_ROW(2, GPIO_PIN_RESET);
    SET_ROW(3, GPIO_PIN_SET);
    SET_ROW(4, GPIO_PIN_SET);

    HAL_Delay(10);

    if (COLUMN_READ(1) == GPIO_PIN_RESET) return '4';
    if (COLUMN_READ(2) == GPIO_PIN_RESET) return '5';
    if (COLUMN_READ(3) == GPIO_PIN_RESET) return '6';
    if (COLUMN_READ(4) == GPIO_PIN_RESET) return 'B';

    // Scan Row 3
    SET_ROW(1, GPIO_PIN_SET);
    SET_ROW(2, GPIO_PIN_SET);
    SET_ROW(3, GPIO_PIN_RESET);
    SET_ROW(4, GPIO_PIN_SET);

    HAL_Delay(10);

    if (COLUMN_READ(1) == GPIO_PIN_RESET) return '7';
    if (COLUMN_READ(2) == GPIO_PIN_RESET) return '8';
    if (COLUMN_READ(3) == GPIO_PIN_RESET) return '9';
    if (COLUMN_READ(4) == GPIO_PIN_RESET) return 'C';

    // Scan Row 4
    SET_ROW(1, GPIO_PIN_SET);
    SET_ROW(2, GPIO_PIN_SET);
    SET_ROW(3, GPIO_PIN_SET);
    SET_ROW(4, GPIO_PIN_RESET);

    HAL_Delay(10);

    if (COLUMN_READ(1) == GPIO_PIN_RESET) return '*';
    if (COLUMN_READ(2) == GPIO_PIN_RESET) return '0';
    if (COLUMN_READ(3) == GPIO_PIN_RESET) return '#';
    if (COLUMN_READ(4) == GPIO_PIN_RESET) return 'D';

    return 0;
}

static void SET_ROW(uint8_t row, GPIO_PinState state)
{
    switch(row)
    {
        case 1:
            HAL_GPIO_WritePin(ROW1_GPIO_PORT, ROW1_PIN, state);
            break;
        case 2:
            HAL_GPIO_WritePin(ROW2_GPIO_PORT, ROW2_PIN, state);
            break;
        case 3:
            HAL_GPIO_WritePin(ROW3_GPIO_PORT, ROW3_PIN, state);
            break;
        case 4:
            HAL_GPIO_WritePin(ROW4_GPIO_PORT, ROW4_PIN, state);
            break;
        default:
            break;
    }
}

static GPIO_PinState COLUMN_READ(uint8_t col)
{
    switch(col)
    {
        case 1:
            return HAL_GPIO_ReadPin(COL1_GPIO_PORT, COL1_PIN);
        case 2:
            return HAL_GPIO_ReadPin(COL2_GPIO_PORT, COL2_PIN);
        case 3:
            return HAL_GPIO_ReadPin(COL3_GPIO_PORT, COL3_PIN);
        case 4:
            return HAL_GPIO_ReadPin(COL4_GPIO_PORT, COL4_PIN);
        default:
            return GPIO_PIN_SET;
    }
}
