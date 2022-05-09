

/*================================================================
 *   
 *   
 *   文件名称：test_uart_a.h
 *   创 建 者：肖飞
 *   创建日期：2022年05月09日 星期一 09时05分16秒
 *   修改日期：2022年05月09日 星期一 10时49分29秒
 *   描    述：
 *
 *================================================================*/
#ifndef _TEST_UART_A_H
#define _TEST_UART_A_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "app_platform.h"
#include "cmsis_os.h"

#include "os_utils.h"
#include "command_status.h"
#include "callback_chain.h"
#include "channels.h"

typedef struct {
	uint8_t id;
	uint32_t v1;
	uint32_t v2;
	uint32_t alive_stamps;
	uint8_t tx_data[64];
	uint16_t tx_size;
	uint8_t rx_data[64];
	uint16_t rx_size;
	command_state_t state;
	callback_item_t data_request_cb;
	callback_item_t data_response_cb;
	callback_item_t periodic_callback_item;
} uart_test_ctx_t;

uart_test_ctx_t *test_uart_a(channels_info_t *channels_info, void *huart, uint8_t id);

#ifdef __cplusplus
}
#endif
#endif //_TEST_UART_A_H
