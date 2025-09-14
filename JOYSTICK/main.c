/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD_I2C.h"
#include "JOYSTICK.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
char buffer[32];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
I2C_HandleTypeDef hi2c1;
/* USER CODE BEGIN PV */
Joystick_TypeDef joystick;
Joystick_Direction current_direction;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
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
  MX_ADC1_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  LCD_INIT();
  JOYSTICK_INIT(&joystick, 100);
  JOYSTICK_CALIBRATECENTER(&joystick, &hadc1, &hadc2); // Calibrate center position
  LCD_SET_CURSOR(0, 0);
  LCD_SEND_STRING("... JOYSTICK ...");
  HAL_Delay(2000);
  LCD_CLEAR();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  JOYSTICK_READ(&joystick, &hadc1, &hadc2);
	  current_direction = JOYSTICK_GETDIRECTION(&joystick);
	  const char* dir_str = JOYSTICK_GETDIRECTIONSTRING(current_direction);
	  LCD_SET_CURSOR(0, 0);
	  sprintf(buffer, "X:%4d Y:%4d", joystick.x_value, joystick.y_value);
	  LCD_SEND_STRING(buffer);
	  LCD_SET_CURSOR(1, 0);
	  sprintf(buffer, "Dir: %-10s", dir_str);
	  LCD_SEND_STRING(buffer);
	  HAL_Delay(200);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
