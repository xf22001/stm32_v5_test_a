

/*================================================================
 *   
 *   
 *   文件名称：test_can_a.h
 *   创 建 者：肖飞
 *   创建日期：2022年05月09日 星期一 09时04分02秒
 *   修改日期：2022年05月09日 星期一 10时44分07秒
 *   描    述：
 *
 *================================================================*/
#ifndef _TEST_CAN_A_H
#define _TEST_CAN_A_H
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
	uint32_t extid;
	u_u64_u32_t send;
	uint32_t stamp;
	uint32_t send_stamp;
	uint32_t alive_stamps;
	command_state_t state;
	callback_item_t data_request_cb;
	callback_item_t data_response_cb;
	callback_item_t periodic_callback_item;
} can_test_ctx_t;

can_test_ctx_t *test_can_a(channels_info_t *channels_info, void *hcan, uint8_t id);

#ifdef __cplusplus
}
#endif
#endif //_TEST_CAN_A_H
