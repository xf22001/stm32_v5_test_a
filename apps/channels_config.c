

/*================================================================
 *
 *
 *   文件名称：channels_config.c
 *   创 建 者：肖飞
 *   创建日期：2021年01月18日 星期一 09时26分44秒
 *   修改日期：2022年07月07日 星期四 13时50分32秒
 *   描    述：
 *
 *================================================================*/
#include "channels_config.h"
#include "os_utils.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern SPI_HandleTypeDef hspi3;

static channels_config_t channels_config_0 = {
	.id = 0,
	.display_config = {
		.huart = &huart6,
		.station = 1,
	},
	.can1 = {
		.hcan = &hcan1,
	},
	.can2 = {
		.hcan = &hcan2,
	},
	.can3 = {
		.hcan = &hspi3,
	},
	.uart1 = {
		.huart = &huart1,
	},
	.uart2 = {
		.huart = &huart2,
	},
	.uart3 = {
		.huart = &huart3,
	},
	.uart4 = {
		.huart = &huart4,
	},
	.comm_config = {
		.huart = &huart5,
	},
};

static channels_config_t *channels_config_sz[] = {
	&channels_config_0,
};

channels_config_t *get_channels_config(uint8_t id)
{
	int i;
	channels_config_t *channels_config = NULL;
	channels_config_t *channels_config_item = NULL;

	for(i = 0; i < ARRAY_SIZE(channels_config_sz); i++) {
		channels_config_item = channels_config_sz[i];

		if(channels_config_item->id == id) {
			channels_config = channels_config_item;
			break;
		}
	}

	return channels_config;
}
