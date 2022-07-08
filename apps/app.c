

/*================================================================
 *
 *
 *   文件名称：app.c
 *   创 建 者：肖飞
 *   创建日期：2019年10月11日 星期五 16时54分03秒
 *   修改日期：2022年07月08日 星期五 09时15分23秒
 *   描    述：
 *
 *================================================================*/
#include "app.h"

#include <string.h>

#include "iwdg.h"
#include "os_utils.h"
#include "test_serial.h"
#include "test_event.h"
#include "test_can.h"
#include "display.h"
#include "probe_tool.h"

#include "sal_socket.h"
#include "sal_netdev.h"
#include "sal_netdev.h"
#if defined(SAL_WIZNET)
#include "wiz_ethernet.h"
#elif defined(SAL_DTU)
#include "dtu_ethernet.h"
#endif
#include "sal_hook.h"

#include "log.h"

extern IWDG_HandleTypeDef hiwdg;

static app_info_t *app_info = NULL;
static os_signal_t app_event = NULL;

app_info_t *get_app_info(void)
{
	return app_info;
}

static void app_event_init(size_t size)
{
	if(app_event != NULL) {
		return;
	}

	app_event = signal_create(size);
	OS_ASSERT(app_event != NULL);
}

void app_init(void)
{
	app_event_init(10);
	mem_info_init();
}

void send_app_event(app_event_t event, uint32_t timeout)
{
	signal_send(app_event, event, timeout);
}

static void app_mechine_info_invalid(void *fn_ctx, void *chain_ctx)
{
	//app_info_t *app_info = (app_info_t *)fn_ctx;
	//modbus_data_ctx_t *modbus_data_ctx = (modbus_data_ctx_t *)chain_ctx;
}

__weak void load_app_display_cache(app_info_t *app_info)
{
}

__weak void sync_app_display_cache(app_info_t *app_info)
{
}

static void app_mechine_info_changed(void *fn_ctx, void *chain_ctx)
{
	app_info_t *app_info = (app_info_t *)fn_ctx;

	sync_app_display_cache(app_info);

	if(app_info->mechine_info_invalid != 0) {
		app_info->mechine_info_invalid = 0;
	}
}

void update_network_ip_config(void)
{
	int exit = 0;

	while(exit == 0) {
		if(set_dhcp_enable(1) != 0) {
			debug("");
			osDelay(1000);
		} else {
			exit = 1;
		}
	}
}


void app(void const *argument)
{
	poll_loop_t *poll_loop;
	display_info_t *display_info = NULL;
	channels_info_t *channels_info;
	int ret;

	app_info = (app_info_t *)os_calloc(1, sizeof(app_info_t));
	OS_ASSERT(app_info != NULL);

	load_app_display_cache(app_info);

	sal_init();
#if defined(SAL_WIZNET)
	wiz_init();
#elif defined(SAL_DTU)
	dtu_init();
#endif

	update_network_ip_config();

	poll_loop = get_or_alloc_poll_loop(0);
	OS_ASSERT(poll_loop != NULL);

	probe_broadcast_add_poll_loop(poll_loop);
	probe_server_add_poll_loop(poll_loop);

	while(is_log_server_valid() == 0) {
		osDelay(1);
	}

	add_log_handler((log_fn_t)log_udp_data);

	debug("===========================================start app============================================");

	channels_info = start_channels();
	OS_ASSERT(channels_info != NULL);

	ntp_client_add_poll_loop(poll_loop);

	display_info = (display_info_t *)channels_info->display_info;
	OS_ASSERT(display_info != NULL);

	if(display_info->modbus_slave_info != NULL) {
		app_info->display_data_invalid_callback_item.fn = app_mechine_info_invalid;
		app_info->display_data_invalid_callback_item.fn_ctx = app_info;
		OS_ASSERT(register_callback(display_info->modbus_slave_info->data_invalid_chain, &app_info->display_data_invalid_callback_item) == 0);

		app_info->display_data_changed_callback_item.fn = app_mechine_info_changed;
		app_info->display_data_changed_callback_item.fn_ctx = app_info;
		OS_ASSERT(register_callback(display_info->modbus_slave_info->data_changed_chain, &app_info->display_data_changed_callback_item) == 0);
	}

	while(1) {
		uint32_t event;
		ret = signal_wait(app_event, &event, 1000);

		if(ret == 0) {
			switch(event) {
				default: {
					debug("unhandled event %x", event);
				}
				break;
			}
		}
	}
}

static uint32_t work_led_blink_periodic = 1000;

void set_work_led_fault_state(uint8_t state)
{
	if(state == 1) {
		work_led_blink_periodic = 125;
	}
}

static void blink_work_led()
{
	static uint32_t ledcpu_stamp = 0;
	uint32_t ticks = osKernelSysTick();

	if(ticks_duration(ticks, ledcpu_stamp) < work_led_blink_periodic) {
		return;
	}

	ledcpu_stamp = ticks;

	HAL_GPIO_TogglePin(SYS_LED_GPIO_Port, SYS_LED_Pin);
}

void idle(void const *argument)
{
	MX_IWDG_Init();

	while(1) {
		HAL_IWDG_Refresh(&hiwdg);
		blink_work_led();
		osDelay(10);
	}
}
