/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "SEV-SEG.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
char buffer[20];
SevenSeg_Config_t seg_config =
{
    .gpio_port = GPIOA,
    .PIN_A = GPIO_PIN_0,
    .PIN_B = GPIO_PIN_1,
    .PIN_C = GPIO_PIN_2,
    .PIN_D = GPIO_PIN_3,
    .PIN_E = GPIO_PIN_4,
    .PIN_F = GPIO_PIN_5,
    .PIN_G = GPIO_PIN_6,
    .PIN_DP = GPIO_PIN_7,
    .type = SEVEN_SEG_COMMON_CATHODE  // Change to SEVEN_SEG_COMMON_ANODE if needed
};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  LCD_INIT();
  SEVEN_SEG_INIT(&seg_config);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  for (uint8_t i = 0; i < 10; i++)
	  {
		  SEVEN_SEG_DISPLAYDIGIT(i);
		  HAL_Delay(500);
	  }

	  // Display hex characters
	  SEVEN_SEG_DISPLAYCHAR('A');
	  HAL_Delay(500);
	  SEVEN_SEG_DISPLAYCHAR('B');
	  HAL_Delay(500);
	  SEVEN_SEG_DISPLAYCHAR('C');
	  HAL_Delay(500);
	  SEVEN_SEG_DISPLAYCHAR('D');
	  HAL_Delay(500);
	  SEVEN_SEG_DISPLAYCHAR('E');
	  HAL_Delay(500);
	  SEVEN_SEG_DISPLAYCHAR('F');
	  HAL_Delay(500);

	  SEVEN_SEG_ENABLEDECIMALPOINT();
	  SEVEN_SEG_DISPLAYDIGIT(5);
	  HAL_Delay(1000);
	  SEVEN_SEG_DISABLEDECIMALPOINT();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

