

/*================================================================
 *
 *
 *   文件名称：channels.c
 *   创 建 者：肖飞
 *   创建日期：2021年01月18日 星期一 09时26分31秒
 *   修改日期：2022年07月04日 星期一 10时34分29秒
 *   描    述：
 *
 *================================================================*/
#include "channels.h"

#include <string.h>
#include <stdlib.h>

#include "object_class.h"
#include "os_utils.h"
#include "app.h"
#include "display.h"
#include "test_can_a.h"
#include "test_uart_a.h"
#include "test_ports_a.h"

#include "log.h"

static object_class_t *channels_class = NULL;

char *get_channels_event_type_des(channels_event_type_t type)
{
	char *des = "unknow";

	switch(type) {
			add_des_case(CHANNELS_EVENT_UNKNOW);

		default: {
		}
		break;
	}

	return des;
}

int set_fault(bitmap_t *faults, int fault, uint8_t v)
{
	return set_bitmap_value(faults, fault, v);
}

int get_fault(bitmap_t *faults, int fault)
{
	return get_bitmap_value(faults, fault);
}

int get_first_fault(bitmap_t *faults)
{
	int ret = get_first_value_index(faults, 1);

	if(ret >= faults->size) {
		ret = -1;
	}

	return ret;
}

static void free_channels_info(channels_info_t *channels_info)
{
	app_panic();
}

static void channels_info_process_event(channels_info_t *channels_info)
{
	channels_event_t *channels_event = NULL;
	int ret = -1;

	if(channels_info == NULL) {
		return;
	}

	ret = event_pool_wait_event(channels_info->event_pool, CHANNEL_TASK_PERIODIC);

	if(ret != 0) {
		return;
	}

	channels_event = event_pool_get_event(channels_info->event_pool);

	if(channels_event != NULL) {
		do_callback_chain(channels_info->common_event_chain, channels_event);

		if(channels_event->event != NULL) {
			os_free(channels_event->event);
		}

		os_free(channels_event);
	}
}

static void channels_info_handle_common_periodic(channels_info_t *channels_info)
{
	do_callback_chain(channels_info->common_periodic_chain, channels_info);
}

static void channels_periodic(channels_info_t *channels_info)
{
	uint32_t ticks = osKernelSysTick();

	if(ticks_duration(ticks, channels_info->periodic_stamp) >= CHANNEL_TASK_PERIODIC) {
		channels_info->periodic_stamp = ticks;
		channels_info_handle_common_periodic(channels_info);
	}
}

static void task_channels(void const *argument)
{
	channels_info_t *channels_info = (channels_info_t *)argument;

	OS_ASSERT(channels_info != NULL);

	while(1) {
		if(channels_info->configed == 0) {
			osDelay(10);
			continue;
		}

		channels_info_process_event(channels_info);

		//处理周期性事件
		channels_periodic(channels_info);
	}
}

int send_channels_event(channels_info_t *channels_info, channels_event_t *channels_event, uint32_t timeout)
{
	return event_pool_put_event(channels_info->event_pool, channels_event, timeout);
}

__weak void channels_modbus_data_action(void *fn_ctx, void *chain_ctx)
{
	//channels_info_t *channels_info = (channels_info_t *)fn_ctx;
	modbus_data_ctx_t *modbus_data_ctx = (modbus_data_ctx_t *)chain_ctx;

	debug("op:%s, addr:%d, value:%d",
	      (modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) ? "get" :
	      (modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) ? "set" :
	      "unknow",
	      modbus_data_ctx->addr,
	      modbus_data_ctx->value);
}

__weak void load_channels_display_cache(channels_info_t *channels_info)
{
}

__weak void sync_channels_display_cache(channels_info_t *channels_info)
{
}

static void channels_info_channels_settings_invalid(void *fn_ctx, void *chain_ctx)
{
}

static void channels_info_channels_settings_changed(void *fn_ctx, void *chain_ctx)
{
	channels_info_t *channels_info = (channels_info_t *)fn_ctx;

	sync_channels_display_cache(channels_info);
}

static void handle_channels_common_event(void *_channels_info, void *_channels_event)
{
	//channels_info_t *channels_info = (channels_info_t *)_channels_info;
	channels_event_t *channels_event = (channels_event_t *)_channels_event;

	debug("channels_info common process event %s!", get_channels_event_type_des(channels_event->type));

	switch(channels_event->type) {
		default: {
		}
		break;
	}
}

static int channels_info_set_channels_config(channels_info_t *channels_info, channels_config_t *channels_config)
{
	int ret = 0;
	display_info_t *display_info;
	debug("use channels channels_config %d!", channels_config->id);

	channels_info->event_pool = alloc_event_pool();

	OS_ASSERT(channels_info->event_pool != NULL);

	alloc_display_info(channels_info);

	display_info = (display_info_t *)channels_info->display_info;
	OS_ASSERT(display_info != NULL);

	osThreadDef(channels, task_channels, osPriorityNormal, 0, 128 * 2 * 2);
	osThreadCreate(osThread(channels), channels_info);

	if(display_info->modbus_slave_info != NULL) {
		channels_info->display_data_action_callback_item.fn = channels_modbus_data_action;
		channels_info->display_data_action_callback_item.fn_ctx = channels_info;
		OS_ASSERT(register_callback(display_info->modbus_slave_info->data_action_chain, &channels_info->display_data_action_callback_item) == 0);

		channels_info->display_data_invalid_callback_item.fn = channels_info_channels_settings_invalid;
		channels_info->display_data_invalid_callback_item.fn_ctx = channels_info;
		OS_ASSERT(register_callback(display_info->modbus_slave_info->data_invalid_chain, &channels_info->display_data_invalid_callback_item) == 0);

		channels_info->display_data_changed_callback_item.fn = channels_info_channels_settings_changed;
		channels_info->display_data_changed_callback_item.fn_ctx = channels_info;
		OS_ASSERT(register_callback(display_info->modbus_slave_info->data_changed_chain, &channels_info->display_data_changed_callback_item) == 0);
	}

	channels_info->can1 = test_can_a(channels_info, channels_info->channels_config->can1.hcan, 0);
	channels_info->can2 = test_can_a(channels_info, channels_info->channels_config->can2.hcan, 1);
	channels_info->can3 = test_can_a(channels_info, channels_info->channels_config->can3.hcan, 2);
	channels_info->uart1 = test_uart_a(channels_info, channels_info->channels_config->uart1.huart, 0);
	channels_info->uart2 = test_uart_a(channels_info, channels_info->channels_config->uart2.huart, 1);
	channels_info->uart3 = test_uart_a(channels_info, channels_info->channels_config->uart3.huart, 2);
	channels_info->uart4 = test_uart_a(channels_info, channels_info->channels_config->uart4.huart, 3);

	channels_info->modbus_master_info = get_or_alloc_modbus_master_info(channels_config->comm_config.huart);
	OS_ASSERT(channels_info->modbus_master_info != NULL);


	channels_info->event_callback_item.fn = handle_channels_common_event;
	channels_info->event_callback_item.fn_ctx = channels_info;
	OS_ASSERT(register_callback(channels_info->common_event_chain, &channels_info->event_callback_item) == 0);

	start_ports_tests(channels_info);

	channels_info->configed = 1;

	return ret;
}

static channels_info_t *alloc_channels_info(channels_config_t *channels_config)
{
	channels_info_t *channels_info = NULL;

	OS_ASSERT(channels_config != NULL);

	channels_info = (channels_info_t *)os_calloc(1, sizeof(channels_info_t));
	OS_ASSERT(channels_info != NULL);

	channels_info->channels_config = channels_config;

	channels_info->common_periodic_chain = alloc_callback_chain();
	OS_ASSERT(channels_info->common_periodic_chain != NULL);
	channels_info->common_event_chain = alloc_callback_chain();
	OS_ASSERT(channels_info->common_event_chain != NULL);
	channels_info->channels_notify_chain = alloc_callback_chain();
	OS_ASSERT(channels_info->channels_notify_chain != NULL);

	channels_info->faults = alloc_bitmap(CHANNELS_FAULT_SIZE);
	OS_ASSERT(channels_info->faults != NULL);

	OS_ASSERT(channels_info_set_channels_config(channels_info, channels_config) == 0);

	return channels_info;
}

static int object_filter(void *o, void *ctx)
{
	int ret = -1;
	channels_info_t *channels_info = (channels_info_t *)o;
	channels_config_t *channels_config = (channels_config_t *)ctx;

	if(channels_info->channels_config == channels_config) {
		ret = 0;
	}

	return ret;
}

static channels_info_t *get_or_alloc_channels_info(channels_config_t *channels_config)
{
	channels_info_t *channels_info = NULL;

	os_enter_critical();

	if(channels_class == NULL) {
		channels_class = object_class_alloc();
	}

	os_leave_critical();

	channels_info = (channels_info_t *)object_class_get_or_alloc_object(channels_class, object_filter, channels_config, (object_alloc_t)alloc_channels_info, (object_free_t)free_channels_info);

	return channels_info;
}

static channels_info_t *_channels_info = NULL;

channels_info_t *start_channels(void)
{
	channels_config_t *channels_config = get_channels_config(0);

	OS_ASSERT(channels_config != NULL);

	_channels_info = get_or_alloc_channels_info(channels_config);
	OS_ASSERT(_channels_info != NULL);

	return _channels_info;
}

channels_info_t *get_channels(void)
{
	return _channels_info;
}
