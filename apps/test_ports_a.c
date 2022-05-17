

/*================================================================
 *
 *
 *   文件名称：test_ports_a.c
 *   创 建 者：肖飞
 *   创建日期：2022年05月16日 星期一 16时36分32秒
 *   修改日期：2022年05月17日 星期二 10时27分09秒
 *   描    述：
 *
 *================================================================*/
#include "test_ports_a.h"
#include "modbus_master_txrx.h"
#include "main.h"

#include "log.h"

typedef enum {
	PORTS_TEST_TYPE_NONE = 0,
	PORTS_TEST_TYPE_CONTACTOR_DRV,
	PORTS_TEST_TYPE_FAN1_RLY_DRV,
	PORTS_TEST_TYPE_VTRANS_1_12V_24,
	PORTS_TEST_TYPE_VTRANS_2_12V_24,
	PORTS_TEST_TYPE_BMSPOWER_PLUG1,
	PORTS_TEST_TYPE_BMSPOWER_PLUG2,
	PORTS_TEST_TYPE_LED_YELLOW_PLUG1,
	PORTS_TEST_TYPE_LED_YELLOW_PLUG2,
	PORTS_TEST_TYPE_LED_RED_PLUG1,
	PORTS_TEST_TYPE_LED_RED_PLUG2,
	PORTS_TEST_TYPE_LED_GREEN_POWER1,
	PORTS_TEST_TYPE_LED_GREEN_POWER2,
	PORTS_TEST_TYPE_CHARGING_LED_SCLK2,
	PORTS_TEST_CHARGING_LED_LCLK2,
	PORTS_TEST_CHARGING_LED_DS2,
	PORTS_TEST_CHARGING_LED_SCLK1,
	PORTS_TEST_CHARGING_LED_LCLK1,
	PORTS_TEST_CHARGING_LED_DS1,
	PORTS_TEST_PAR_EXT_RX1,
	PORTS_TEST_PAR_EXT_RX2,
	PORTS_TEST_PAR_EXT_RX3,
	PORTS_TEST_PAR_EXT_TX1,
	PORTS_TEST_PAR_EXT_TX2,
	PORTS_TEST_PAR_EXT_TX3,
} ports_test_type_t;

typedef struct {
	callback_item_t periodic_callback_item;
	modbus_master_info_t *modbus_master_info;
	uint8_t state;
	int port_fault;
	uint32_t stamp;
	ports_test_type_t ports_test_type;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t gpio_state1;
	uint8_t gpio_state2;
	int index;
} test_ports_ctx_t;

typedef struct {
	ports_test_type_t request_ports_test_type;
	int port_fault;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t gpio_state1;
	uint8_t gpio_state2;
} test_port_item_t;

static test_port_item_t test_port_items[] = {
	{
		.request_ports_test_type = PORTS_TEST_TYPE_CONTACTOR_DRV,
		.port_fault = CHANNELS_FAULT_CONTACTOR_DRV,
		.gpio_port = CONTACTOR_DRV_GPIO_Port,
		.gpio_pin = CONTACTOR_DRV_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_FAN1_RLY_DRV,
		.port_fault = CHANNELS_FAULT_FAN1_RLY_DRV,
		.gpio_port = FAN1_RLY_DRV_GPIO_Port,
		.gpio_pin = FAN1_RLY_DRV_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_VTRANS_1_12V_24,
		.port_fault = CHANNELS_FAULT_VTRANS_1_12V_24,
		.gpio_port = VTRANS_1_12V_24_GPIO_Port,
		.gpio_pin = VTRANS_1_12V_24_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_VTRANS_2_12V_24,
		.port_fault = CHANNELS_FAULT_VTRANS_2_12V_24,
		.gpio_port = VTRANS_2_12V_24_GPIO_Port,
		.gpio_pin = VTRANS_2_12V_24_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_BMSPOWER_PLUG1,
		.port_fault = CHANNELS_FAULT_BMSPOWER_PLUG1,
		.gpio_port = BMSPOWER_PLUG1_GPIO_Port,
		.gpio_pin = BMSPOWER_PLUG1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_BMSPOWER_PLUG2,
		.port_fault = CHANNELS_FAULT_BMSPOWER_PLUG2,
		.gpio_port = BMSPOWER_PLUG2_GPIO_Port,
		.gpio_pin = BMSPOWER_PLUG2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_LED_YELLOW_PLUG1,
		.port_fault = CHANNELS_FAULT_LED_YELLOW_PLUG1,
		.gpio_port = LED_YELLOW_PLUG1_GPIO_Port,
		.gpio_pin = LED_YELLOW_PLUG1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_LED_YELLOW_PLUG2,
		.port_fault = CHANNELS_FAULT_LED_YELLOW_PLUG2,
		.gpio_port = LED_YELLOW_PLUG2_GPIO_Port,
		.gpio_pin = LED_YELLOW_PLUG2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_LED_RED_PLUG1,
		.port_fault = CHANNELS_FAULT_LED_RED_PLUG1,
		.gpio_port = LED_RED_PLUG1_GPIO_Port,
		.gpio_pin = LED_RED_PLUG1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_LED_RED_PLUG2,
		.port_fault = CHANNELS_FAULT_LED_RED_PLUG2,
		.gpio_port = LED_RED_PLUG2_GPIO_Port,
		.gpio_pin = LED_RED_PLUG2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_LED_GREEN_POWER1,
		.port_fault = CHANNELS_FAULT_LED_GREEN_POWER1,
		.gpio_port = LED_GREEN_POWER1_GPIO_Port,
		.gpio_pin = LED_GREEN_POWER1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_LED_GREEN_POWER2,
		.port_fault = CHANNELS_FAULT_LED_GREEN_POWER2,
		.gpio_port = LED_GREEN_POWER2_GPIO_Port,
		.gpio_pin = LED_GREEN_POWER2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_TYPE_CHARGING_LED_SCLK2,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_SCLK2,
		.gpio_port = CHARGING_LED_SCLK2_GPIO_Port,
		.gpio_pin = CHARGING_LED_SCLK2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_CHARGING_LED_LCLK2,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_LCLK2,
		.gpio_port = CHARGING_LED_LCLK2_GPIO_Port,
		.gpio_pin = CHARGING_LED_LCLK2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_CHARGING_LED_DS2,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_DS2,
		.gpio_port = CHARGING_LED_DS2_GPIO_Port,
		.gpio_pin = CHARGING_LED_DS2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_CHARGING_LED_SCLK1,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_SCLK1,
		.gpio_port = CHARGING_LED_SCLK1_GPIO_Port,
		.gpio_pin = CHARGING_LED_SCLK1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_CHARGING_LED_LCLK1,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_LCLK1,
		.gpio_port = CHARGING_LED_LCLK1_GPIO_Port,
		.gpio_pin = CHARGING_LED_LCLK1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_CHARGING_LED_DS1,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_DS1,
		.gpio_port = CHARGING_LED_DS1_GPIO_Port,
		.gpio_pin = CHARGING_LED_DS1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_PAR_EXT_RX1,
		.port_fault = CHANNELS_FAULT_PAR_EXT_RX1,
		.gpio_port = PAR_EXT_RX1_GPIO_Port,
		.gpio_pin = PAR_EXT_RX1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_PAR_EXT_RX2,
		.port_fault = CHANNELS_FAULT_PAR_EXT_RX2,
		.gpio_port = PAR_EXT_RX2_GPIO_Port,
		.gpio_pin = PAR_EXT_RX2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_PAR_EXT_RX3,
		.port_fault = CHANNELS_FAULT_PAR_EXT_RX3,
		.gpio_port = PAR_EXT_RX3_GPIO_Port,
		.gpio_pin = PAR_EXT_RX3_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_PAR_EXT_TX1,
		.port_fault = CHANNELS_FAULT_PAR_EXT_TX1,
		.gpio_port = PAR_EXT_TX1_GPIO_Port,
		.gpio_pin = PAR_EXT_TX1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_PAR_EXT_TX2,
		.port_fault = CHANNELS_FAULT_PAR_EXT_TX2,
		.gpio_port = PAR_EXT_TX2_GPIO_Port,
		.gpio_pin = PAR_EXT_TX2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_ports_test_type = PORTS_TEST_PAR_EXT_TX3,
		.port_fault = CHANNELS_FAULT_PAR_EXT_TX3,
		.gpio_port = PAR_EXT_TX3_GPIO_Port,
		.gpio_pin = PAR_EXT_TX3_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
};

static int do_port_test(test_ports_ctx_t *ctx, channels_info_t *channels_info)
{
	int ret = 1;

	if(ctx->ports_test_type == PORTS_TEST_TYPE_NONE) {
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

			if(modbus_master_read_items_retry(ctx->modbus_master_info, 1, ctx->ports_test_type, 1, &value, 3) == 0) {
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

			if(modbus_master_read_items_retry(ctx->modbus_master_info, 1, ctx->ports_test_type, 1, &value, 3) == 0) {
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

void handle_ports_test(test_ports_ctx_t *ctx, channels_info_t *channels_info)
{
	test_port_item_t *test_port_item;

	if(ctx->ports_test_type != PORTS_TEST_TYPE_NONE) {
		return;
	}

	if(ARRAY_SIZE(test_port_items) <= 0) {
		return;
	}

	if(ctx->index >= ARRAY_SIZE(test_port_items)) {
		ctx->index = 0;
	}

	test_port_item = &test_port_items[ctx->index];

	ctx->state = 0;
	ctx->ports_test_type = test_port_item->request_ports_test_type;
	ctx->port_fault = test_port_item->port_fault;
	ctx->gpio_port = test_port_item->gpio_port;
	ctx->gpio_pin = test_port_item->gpio_pin;
	ctx->gpio_state1 = test_port_item->gpio_state1;
	ctx->gpio_state2 = test_port_item->gpio_state2;
}

static void ports_test_periodic(void *fn_ctx, void *chain_ctx)
{
	test_ports_ctx_t *ctx = (test_ports_ctx_t *)fn_ctx;
	channels_info_t *channels_info = (channels_info_t *)chain_ctx;
	int ret;

	ret = do_port_test(ctx, channels_info);

	if(ret == -1) {
		set_fault(channels_info->faults, ctx->port_fault, 1);
		ctx->ports_test_type = PORTS_TEST_TYPE_NONE;
	} else if(ret == 0) {
		set_fault(channels_info->faults, ctx->port_fault, 0);
		ctx->ports_test_type = PORTS_TEST_TYPE_NONE;
	}

	handle_ports_test(ctx, channels_info);
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
