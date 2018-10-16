#include "main.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usart.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//	uint8_t buffer[8];
	if (htim->Instance == htim3.Instance)
	{
		///@todo 在这里补充输出函数
		HAL_GPIO_TogglePin(GPIOB, LED3_Pin|LED2_Pin|LED1_Pin);
	  MTiRead();
		MTidatadeal();
//		HAL_SPI_Receive(&hspi1,(uint8_t*)&buffer[0],8,100);
	}
}
/* USER CODE BEGIN 1 */
/**
  * ????: ???c???printf?DEBUG_USARTx
  * ????: ?
  * ? ? ?: ?
  * ?    ?:?
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart5, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * ????: ???c???getchar,scanf?DEBUG_USARTx
  * ????: ?
  * ? ? ?: ?
  * ?    ?:?
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart5,&ch, 1, 0xffff);
  return ch;
}
