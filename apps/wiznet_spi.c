

/*================================================================
 *
 *
 *   文件名称：wiznet_spi.c
 *   创 建 者：肖飞
 *   创建日期：2021年05月19日 星期三 09时47分16秒
 *   修改日期：2021年05月24日 星期一 08时39分36秒
 *   描    述：
 *
 *================================================================*/
#include "app_platform.h"
#include "cmsis_os.h"

#include <wizchip_socket.h>
#include "DHCP/wizchip_dhcp.h"

#include "os_utils.h"
#include "spi_txrx.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;
static spi_info_t *spi_info = NULL;

int wiznet_spi_init(void)
{
	int ret = 0;
	spi_info = get_or_alloc_spi_info(&hspi1);
	OS_ASSERT(spi_info != NULL);
	return ret;
}

void wiznet_spi_write_byte(uint8_t data)
{
	int ret = spi_tx_data(spi_info, &data, 1, 10);

	if(ret != 0) {
		//debug("");
	}
}

uint8_t wiznet_spi_read_byte(void)
{
	uint8_t byte = 0;

	int ret = spi_rx_data(spi_info, &byte, 1, 10);

	if(ret != 0) {
		//debug("");
	}

	return byte;
}

void wiznet_spi_write_burst(uint8_t *pbuf, uint16_t len)
{
	int ret = spi_tx_data(spi_info, pbuf, len, 10);

	if(ret != 0) {
		//debug("");
	}
}

void wiznet_spi_read_burst(uint8_t *pbuf, uint16_t len)
{
	int ret = spi_rx_data(spi_info, pbuf, len, 10);

	if(ret != 0) {
		//debug("");
	}
}

void wiznet_spi_cs_select(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void wiznet_spi_cs_deselect(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void wiz_reset(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
	osDelay(2);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
	osDelay(2);
}
