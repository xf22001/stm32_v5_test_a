

/*================================================================
 *
 *
 *   文件名称：channels.h
 *   创 建 者：肖飞
 *   创建日期：2021年01月18日 星期一 10时08分44秒
 *   修改日期：2022年05月07日 星期六 10时45分06秒
 *   描    述：
 *
 *================================================================*/
#ifndef _CHANNELS_H
#define _CHANNELS_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "app_platform.h"
#include "cmsis_os.h"

#include "event_helper.h"
#include "channels_config.h"

#include "callback_chain.h"
#include "bitmap_ops.h"

#include "display_cache.h"

#define CHANNEL_TASK_PERIODIC (10)

//all channels event type
typedef enum {
	CHANNELS_EVENT_UNKNOW = 0,
} channels_event_type_t;

//all channels event typedef
typedef struct {
	channels_event_type_t type;
	void *event;
} channels_event_t;

typedef enum {
	CHANNELS_FAULT_DISPLAY = 0,
	CHANNELS_FAULT_CAN1,
	CHANNELS_FAULT_CAN2,
	CHANNELS_FAULT_CAN3,
	CHANNELS_FAULT_UART1,
	CHANNELS_FAULT_UART2,
	CHANNELS_FAULT_UART3,
	CHANNELS_FAULT_UART4,
	CHANNELS_FAULT_UART5,
	CHANNELS_FAULT_SIZE,
} channels_fault_t;

typedef enum {
	CHANNELS_NOTIFY_NONE = 0,
} channels_notify_t;

typedef struct {
	channels_notify_t notify;
	void *ctx;
} channels_notify_ctx_t;

typedef struct {
	channels_config_t *channels_config;

	event_pool_t *event_pool;
	callback_chain_t *common_periodic_chain;
	callback_chain_t *common_event_chain;
	callback_chain_t *channels_notify_chain;

	callback_item_t periodic_callback_item;
	callback_item_t event_callback_item;

	callback_item_t display_data_action_callback_item;
	callback_item_t display_data_invalid_callback_item;
	callback_item_t display_data_changed_callback_item;

	uint8_t configed;

	uint32_t periodic_stamp;

	display_cache_channels_t display_cache_channels;
	bitmap_t *faults;//channels_fault_t

	void *display_info;
	void *can1;
	void *can2;
	void *can3;
	void *uart1;
	void *uart2;
	void *uart3;
	void *uart4;
	void *uart5;
} channels_info_t;

char *get_channels_event_type_des(channels_event_type_t type);
int set_fault(bitmap_t *faults, int fault, uint8_t v);
int get_fault(bitmap_t *faults, int fault);
int get_first_fault(bitmap_t *faults);
int send_channels_event(channels_info_t *channels_info, channels_event_t *channels_event, uint32_t timeout);
void channels_modbus_data_action(void *fn_ctx, void *chain_ctx);
void load_channels_display_cache(channels_info_t *channels_info);
void sync_channels_display_cache(channels_info_t *channels_info);
channels_info_t *start_channels(void);
channels_info_t *get_channels(void);

#ifdef __cplusplus
}
#endif

#endif //_CHANNELS_H
