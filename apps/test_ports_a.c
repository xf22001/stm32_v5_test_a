

/*================================================================
 *
 *
 *   文件名称：test_ports_a.c
 *   创 建 者：肖飞
 *   创建日期：2022年05月16日 星期一 16时36分32秒
 *   修改日期：2022年05月16日 星期一 17时27分42秒
 *   描    述：
 *
 *================================================================*/
#include "test_ports_a.h"
#include "modbus_master_txrx.h"
#include "main.h"

typedef struct {
	callback_item_t periodic_callback_item;
	modbus_master_info_t *modbus_master_info;
	uint8_t state;
	int port_fault;
	uint32_t stamp;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t gpio_state1;
	uint8_t gpio_state2;
} test_ports_ctx_t;

static int is_port_test_type(channels_info_t *channels_info)
{
	int ret = 0;

	switch(channels_info->test_type) {
		case CHANNELS_TEST_TYPE_CONTACTOR_DRV:
		case CHANNELS_TEST_TYPE_FAN1_RLY_DRV:
		case CHANNELS_TEST_TYPE_VTRANS_1_12V_24:
		case CHANNELS_TEST_TYPE_VTRANS_2_12V_24:
		case CHANNELS_TEST_TYPE_BMSPOWER_PLUG1:
		case CHANNELS_TEST_TYPE_BMSPOWER_PLUG2:
		case CHANNELS_TEST_TYPE_LED_YELLOW_PLUG1:
		case CHANNELS_TEST_TYPE_LED_YELLOW_PLUG2:
		case CHANNELS_TEST_TYPE_LED_RED_PLUG1:
		case CHANNELS_TEST_TYPE_LED_RED_PLUG2:
		case CHANNELS_TEST_TYPE_LED_GREEN_POWER1:
		case CHANNELS_TEST_TYPE_LED_GREEN_POWER2:
		case CHANNELS_TEST_TYPE_CHARGING_LED_SCLK2:
		case CHANNELS_TEST_TYPE_PAR_EXT_TX2:
		case CHANNELS_TEST_TYPE_PAR_EXT_TX3: {
			ret = 1;
		}
		break;

		default: {
		}
		break;
	}

	return ret;
}

static int do_port_test(test_ports_ctx_t *ctx, channels_info_t *channels_info)
{
	int ret = 1;

	if(is_port_test_type(channels_info) == 0) {
		return ret;
	}

	switch(ctx->state) {
		case 0: {
			HAL_GPIO_WritePin(ctx->gpio_port, ctx->gpio_pin, GPIO_PIN_RESET);
			ctx->stamp = osKernelSysTick();
			ctx->state = 1;
		}
		break;

		case 1: {
			uint16_t value;

			if(modbus_master_read_items_retry(ctx->modbus_master_info, 1, channels_info->test_type, 1, &value, 3) == 0) {
				if(value == ctx->gpio_state1) {
					ctx->state = 2;
				} else {
					ret = -1;
				}
			} else {
				ret = -1;
			}

			if(ticks_duration(osKernelSysTick(), ctx->stamp) > 1000) {
				ret = -1;
			}
		}
		break;

		case 2: {
			HAL_GPIO_WritePin(ctx->gpio_port, ctx->gpio_pin, GPIO_PIN_SET);
			ctx->stamp = osKernelSysTick();
			ctx->state = 3;
		}
		break;

		case 3: {
			uint16_t value;

			if(modbus_master_read_items_retry(ctx->modbus_master_info, 1, channels_info->test_type, 1, &value, 3) == 0) {
				if(value == ctx->gpio_state2) {
					ret = 0;
				} else {
					ret = -1;
				}
			} else {
				ret = -1;
			}

			if(ticks_duration(osKernelSysTick(), ctx->stamp) > 1000) {
				ret = -1;
			}
		}
		break;

		default: {
			app_panic();
		}
		break;
	}

	return ret;
}

static void handle_port_test_request(test_ports_ctx_t *ctx, channels_info_t *channels_info)
{
	if(channels_info->test_type != CHANNELS_TEST_TYPE_NONE) {
		return;
	}

	switch(channels_info->request_test_type) {
		case CHANNELS_TEST_TYPE_CONTACTOR_DRV: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_CONTACTOR_DRV;
			ctx->gpio_port = CONTACTOR_DRV_GPIO_Port;
			ctx->gpio_pin = CONTACTOR_DRV_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_FAN1_RLY_DRV: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_FAN1_RLY_DRV;
			ctx->gpio_port = FAN1_RLY_DRV_GPIO_Port;
			ctx->gpio_pin = FAN1_RLY_DRV_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_VTRANS_1_12V_24: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_VTRANS_1_12V_24;
			ctx->gpio_port = VTRANS_1_12V_24_GPIO_Port;
			ctx->gpio_pin = VTRANS_1_12V_24_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_VTRANS_2_12V_24: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_VTRANS_2_12V_24;
			ctx->gpio_port = VTRANS_2_12V_24_GPIO_Port;
			ctx->gpio_pin = VTRANS_2_12V_24_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_BMSPOWER_PLUG1: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_BMSPOWER_PLUG1;
			ctx->gpio_port = BMSPOWER_PLUG1_GPIO_Port;
			ctx->gpio_pin = BMSPOWER_PLUG1_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_BMSPOWER_PLUG2: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_BMSPOWER_PLUG2;
			ctx->gpio_port = BMSPOWER_PLUG2_GPIO_Port;
			ctx->gpio_pin = BMSPOWER_PLUG2_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_LED_YELLOW_PLUG1: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_LED_YELLOW_PLUG1;
			ctx->gpio_port = LED_YELLOW_PLUG1_GPIO_Port;
			ctx->gpio_pin = LED_YELLOW_PLUG1_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_LED_YELLOW_PLUG2: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_LED_YELLOW_PLUG2;
			ctx->gpio_port = LED_YELLOW_PLUG2_GPIO_Port;
			ctx->gpio_pin = LED_YELLOW_PLUG2_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_LED_RED_PLUG1: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_LED_RED_PLUG1;
			ctx->gpio_port = LED_RED_PLUG1_GPIO_Port;
			ctx->gpio_pin = LED_RED_PLUG1_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_LED_RED_PLUG2: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_LED_RED_PLUG2;
			ctx->gpio_port = LED_RED_PLUG2_GPIO_Port;
			ctx->gpio_pin = LED_RED_PLUG2_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_LED_GREEN_POWER1: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_LED_GREEN_POWER1;
			ctx->gpio_port = LED_GREEN_POWER1_GPIO_Port;
			ctx->gpio_pin = LED_GREEN_POWER1_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_LED_GREEN_POWER2: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_LED_GREEN_POWER2;
			ctx->gpio_port = LED_GREEN_POWER2_GPIO_Port;
			ctx->gpio_pin = LED_GREEN_POWER2_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_CHARGING_LED_SCLK2: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_CHARGING_LED_SCLK2;
			ctx->gpio_port = CHARGING_LED_SCLK2_GPIO_Port;
			ctx->gpio_pin = CHARGING_LED_SCLK2_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_PAR_EXT_TX2: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_PAR_EXT_TX2;
			ctx->gpio_port = PAR_EXT_TX2_GPIO_Port;
			ctx->gpio_pin = PAR_EXT_TX2_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		case CHANNELS_TEST_TYPE_PAR_EXT_TX3: {
			channels_info->test_type = channels_info->request_test_type;
			channels_info->request_test_type = CHANNELS_TEST_TYPE_NONE;

			ctx->state = 0;
			ctx->port_fault = CHANNELS_FAULT_PAR_EXT_TX3;
			ctx->gpio_port = PAR_EXT_TX3_GPIO_Port;
			ctx->gpio_pin = PAR_EXT_TX3_Pin;
			ctx->gpio_state1 = GPIO_PIN_RESET;
			ctx->gpio_state2 = GPIO_PIN_SET;
			set_fault(channels_info->faults, ctx->port_fault, 0);
		}
		break;

		default: {
		}
		break;
	}
}

static void ports_test_periodic(void *fn_ctx, void *chain_ctx)
{
	test_ports_ctx_t *ctx = (test_ports_ctx_t *)fn_ctx;
	channels_info_t *channels_info = (channels_info_t *)chain_ctx;
	int ret;

	handle_port_test_request(ctx, channels_info);

	ret = do_port_test(ctx, channels_info);

	if(ret == -1) {
		set_fault(channels_info->faults, ctx->port_fault, 1);
		channels_info->test_type = CHANNELS_TEST_TYPE_NONE;
	} else if(ret == 0) {
		channels_info->test_type = CHANNELS_TEST_TYPE_NONE;
	}
}

void start_ports_tests(channels_info_t *channels_info)
{
	test_ports_ctx_t *ctx = os_calloc(1, sizeof(test_ports_ctx_t));
	channels_config_t *channels_config = channels_info->channels_config;

	OS_ASSERT(ctx != NULL);

	ctx->modbus_master_info = get_or_alloc_modbus_master_info(channels_config->comm_config.huart);
	OS_ASSERT(ctx->modbus_master_info != NULL);

	ctx->periodic_callback_item.fn = ports_test_periodic;
	ctx->periodic_callback_item.fn_ctx = ctx;
	OS_ASSERT(register_callback(channels_info->common_periodic_chain, &ctx->periodic_callback_item) == 0);
}
