#include "SEV-SEG.h"

static SevenSeg_Config_t* seg_config;
static uint8_t decimal_point_enabled = 0;

// Segment patterns for common cathode (0 = off, 1 = on)
// Order: A, B, C, D, E, F, G, DP
static const uint8_t digit_patterns_cc[] =
{
    0x3F, // 0: ABCDEF
    0x06, // 1: BC
    0x5B, // 2: ABDEG
    0x4F, // 3: ABCDG
    0x66, // 4: BCFG
    0x6D, // 5: ACDFG
    0x7D, // 6: ACDEFG
    0x07, // 7: ABC
    0x7F, // 8: ABCDEFG
    0x6F, // 9: ABCDFG
    0x77, // A: ABCEFG
    0x7C, // B: CDEFG
    0x39, // C: ADEF
    0x5E, // D: BCDEG
    0x79, // E: ADEFG
    0x71  // F: AEFG
};

// For common anode, we'll invert the patterns
static const uint8_t digit_patterns_ca[] =
{
    0xC0, // 0: ABCDEF (inverted)
    0xF9, // 1: BC (inverted)
    0xA4, // 2: ABDEG (inverted)
    0xB0, // 3: ABCDG (inverted)
    0x99, // 4: BCFG (inverted)
    0x92, // 5: ACDFG (inverted)
    0x82, // 6: ACDEFG (inverted)
    0xF8, // 7: ABC (inverted)
    0x80, // 8: ABCDEFG (inverted)
    0x90, // 9: ABCDFG (inverted)
    0x88, // A: ABCEFG (inverted)
    0x83, // B: CDEFG (inverted)
    0xC6, // C: ADEF (inverted)
    0xA1, // D: BCDEG (inverted)
    0x86, // E: ADEFG (inverted)
    0x8E  // F: AEFG (inverted)
};

void SEVEN_SEG_INIT(SevenSeg_Config_t* config)
{
    seg_config 							= config;
    GPIO_InitTypeDef GPIO_InitStruct 	= {0};
    GPIO_InitStruct.Mode 				= GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull 				= GPIO_NOPULL;
    GPIO_InitStruct.Speed 				= GPIO_SPEED_FREQ_LOW;
    uint16_t pins[] =
    {
        config->PIN_A, config->PIN_B, config->PIN_C, config->PIN_D,
        config->PIN_E, config->PIN_F, config->PIN_G, config->PIN_DP
    };
    for (int i = 0; i < 8; i++)
    {
        GPIO_InitStruct.Pin = pins[i];
        HAL_GPIO_Init(config->gpio_port, &GPIO_InitStruct);
    }
    SEVEN_SEG_CLEAR();
}

void SEVEN_SEG_DISPLAYDIGIT(uint8_t digit)
{
    if (digit > 15) digit = 15; // Limit to 0-15 (hex digits)
    uint8_t pattern;
    const uint8_t* patterns;
    if (seg_config->type == SEVEN_SEG_COMMON_CATHODE)
    {
        patterns = digit_patterns_cc;
    }
    else
    {
        patterns = digit_patterns_ca;
    }
    pattern = patterns[digit];
    // Set segment pins according to pattern
    HAL_GPIO_WritePin(seg_config->gpio_port, seg_config->PIN_A, (pattern & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(seg_config->gpio_port, seg_config->PIN_B, (pattern & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(seg_config->gpio_port, seg_config->PIN_C, (pattern & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(seg_config->gpio_port, seg_config->PIN_D, (pattern & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(seg_config->gpio_port, seg_config->PIN_E, (pattern & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(seg_config->gpio_port, seg_config->PIN_F, (pattern & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(seg_config->gpio_port, seg_config->PIN_G, (pattern & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    // Handle decimal point
    if (decimal_point_enabled)
    {
        HAL_GPIO_WritePin(seg_config->gpio_port, seg_config->PIN_DP, (seg_config->type == SEVEN_SEG_COMMON_CATHODE) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(seg_config->gpio_port, seg_config->PIN_DP, (seg_config->type == SEVEN_SEG_COMMON_CATHODE) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}

void SEVEN_SEG_DISPLAYCHAR(char character)
{
    uint8_t digit;
    switch (character)
    {
        case '0'...'9': digit = character - '0'; break;
        case 'A'...'F': digit = character - 'A' + 10; break;
        case 'a'...'f': digit = character - 'a' + 10; break;
        default: digit = 0; // Display 0 for invalid characters
    }
    SEVEN_SEG_DISPLAYDIGIT(digit);
}

void SEVEN_SEG_DISPLAYHEX(uint8_t hex_value)
{
	SEVEN_SEG_DISPLAYDIGIT(hex_value & 0x0F);
}

void SEVEN_SEG_CLEAR(void)
{
    // Turn off all segments
    uint16_t pins[] =
    {
		seg_config->PIN_A, seg_config->PIN_B, seg_config->PIN_C, seg_config->PIN_D,
		seg_config->PIN_E, seg_config->PIN_F, seg_config->PIN_G, seg_config->PIN_DP
    };
    for (int i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(seg_config->gpio_port, pins[i], (seg_config->type == SEVEN_SEG_COMMON_CATHODE) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}

void SEVEN_SEG_ENABLEDECIMALPOINT(void)
{
    decimal_point_enabled = 1;
}

void SEVEN_SEG_DISABLEDECIMALPOINT(void)
{
    decimal_point_enabled = 0;
}

uint8_t SEVEN_SEG_DIGITTOSEGMENTS(uint8_t digit)
{
    if (digit > 15) digit = 15;
    if (seg_config->type == SEVEN_SEG_COMMON_CATHODE)
    {
        return digit_patterns_cc[digit];
    }
    else
    {
        return digit_patterns_ca[digit];
    }
}

uint8_t SEVEN_SEG_CHARTOSEGMENTS(char character)
{
    uint8_t digit;
    switch (character)
    {
        case '0'...'9': digit = character - '0'; break;
        case 'A'...'F': digit = character - 'A' + 10; break;
        case 'a'...'f': digit = character - 'a' + 10; break;
        default: digit = 0;
    }
    return SEVEN_SEG_DIGITTOSEGMENTS(digit);
}
