/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "SERVO-MOTOR.h"
#include "LCD_I2C.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
char buffer[20];
Servo_HandleTypeDef my_servo;
int angle;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim2;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  LCD_INIT();
  SERVO_INIT(&my_servo, &htim2, TIM_CHANNEL_2, 500, 2500, 0, 180);
  LCD_SET_CURSOR(0, 0);
  LCD_SEND_STRING(".. Servo Motor ..");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  for (angle = 0; angle <= 180; angle += 10)
	  {
  		SERVO_SETANGLE(&my_servo, angle);
  		snprintf(buffer, sizeof(buffer), "Angle : %4d", angle);
  		LCD_SET_CURSOR(1, 0);
  		LCD_SEND_STRING(buffer);
  		HAL_Delay(200);
	  }

	  for (angle = 180; angle >= 0; angle -= 10)
	  {
  		SERVO_SETANGLE(&my_servo, angle);
  		snprintf(buffer, sizeof(buffer), "Angle : %4d", angle);
  		LCD_SET_CURSOR(1, 0);
  		LCD_SEND_STRING(buffer);
  		HAL_Delay(200);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

