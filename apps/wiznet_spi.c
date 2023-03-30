

/*================================================================
 *
 *
 *   文件名称：wiznet_spi.c
 *   创 建 者：肖飞
 *   创建日期：2021年05月19日 星期三 09时47分16秒
 *   修改日期：2023年03月14日 星期二 14时33分02秒
 *   描    述：
 *
 *================================================================*/
#include "app_platform.h"
#include "cmsis_os.h"

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
	int ret = -1;

	OS_ASSERT(spi_info != NULL);
	OS_ASSERT(spi_info->claimed == 1);
	ret = spi_tx_data_unlocked(spi_info, &data, 1, 10);

	if(ret != 0) {
		//debug("");
	}
}

uint8_t wiznet_spi_read_byte(void)
{
	uint8_t byte = 0;
	int ret = -1;

	OS_ASSERT(spi_info != NULL);
	OS_ASSERT(spi_info->claimed == 1);
	ret = spi_rx_data_unlocked(spi_info, &byte, 1, 10);

	if(ret != 0) {
		//debug("");
	}

	return byte;
}

void wiznet_spi_write_burst(uint8_t *pbuf, uint16_t len)
{
	int ret = -1;

	OS_ASSERT(spi_info != NULL);
	OS_ASSERT(spi_info->claimed == 1);
	ret = spi_tx_data_unlocked(spi_info, pbuf, len, 10);

	if(ret != 0) {
		//debug("");
	}
}

void wiznet_spi_read_burst(uint8_t *pbuf, uint16_t len)
{
	int ret = -1;

	OS_ASSERT(spi_info != NULL);
	OS_ASSERT(spi_info->claimed == 1);
	ret = spi_rx_data_unlocked(spi_info, pbuf, len, 10);

	if(ret != 0) {
		//debug("");
	}
}

void wiznet_spi_cs_select(void)
{
	OS_ASSERT(spi_info != NULL);
	spi_claim(spi_info, ETH_CS_GPIO_Port, ETH_CS_Pin);
}

void wiznet_spi_cs_deselect(void)
{
	OS_ASSERT(spi_info != NULL);
	spi_unclaim(spi_info, ETH_CS_GPIO_Port, ETH_CS_Pin);
}

void wiz_reset(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
	osDelay(2);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
	osDelay(2);
}
