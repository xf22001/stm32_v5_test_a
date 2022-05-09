

/*================================================================
 *
 *
 *   文件名称：exti.c
 *   创 建 者：肖飞
 *   创建日期：2021年01月26日 星期二 14时22分36秒
 *   修改日期：2022年01月23日 星期日 14时32分12秒
 *   描    述：
 *
 *================================================================*/
#include "app_platform.h"
#include "cmsis_os.h"

void spi_can_isr(void *hcan);
extern SPI_HandleTypeDef hspi3;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin) {
		//case isr_pin_Pin: {
		//	wiz_isr();
		//}
		//break;
		case GPIO_PIN_8: {
			spi_can_isr(&hspi3);
		}
		break;

		default: {
		}
		break;
	}
}
