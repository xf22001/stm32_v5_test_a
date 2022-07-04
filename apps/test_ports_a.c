

/*================================================================
 *
 *
 *   文件名称：test_ports_a.c
 *   创 建 者：肖飞
 *   创建日期：2022年05月16日 星期一 16时36分32秒
 *   修改日期：2022年07月04日 星期一 17时13分12秒
 *   描    述：
 *
 *================================================================*/
#include "test_ports_a.h"
#include "main.h"
#include "test_type.h"

#include "log.h"

typedef struct {
	int index;
	uint8_t state;
	uint32_t stamp;

	test_type_t test_type_ports;
	int port_fault;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t gpio_state1;
	uint8_t gpio_state2;
} test_ports_output_ctx_t;

typedef struct {
	int index;
	uint8_t state;
	uint32_t stamp;

	test_type_t test_type_ports;
	int port_fault;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t gpio_state1;
	uint8_t gpio_state2;
} test_ports_input_ctx_t;

typedef struct {
	callback_item_t periodic_callback_item;
	test_ports_output_ctx_t test_ports_output_ctx;
	test_ports_input_ctx_t test_ports_input_ctx;
} test_ports_ctx_t;

typedef struct {
	test_type_t request_test_type_ports;
	int port_fault;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t gpio_state1;
	uint8_t gpio_state2;
} test_port_output_item_t;

static test_port_output_item_t test_port_output_items[] = {
	{
		.request_test_type_ports = TEST_TYPE_PORTS_CONTACTOR_DRV,
		.port_fault = CHANNELS_FAULT_CONTACTOR_DRV,
		.gpio_port = CONTACTOR_DRV_GPIO_Port,
		.gpio_pin = CONTACTOR_DRV_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_FAN1_RLY_DRV,
		.port_fault = CHANNELS_FAULT_FAN1_RLY_DRV,
		.gpio_port = FAN1_RLY_DRV_GPIO_Port,
		.gpio_pin = FAN1_RLY_DRV_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_VTRANS_1_12V_24,
		.port_fault = CHANNELS_FAULT_VTRANS_1_12V_24,
		.gpio_port = VTRANS_1_12V_24_GPIO_Port,
		.gpio_pin = VTRANS_1_12V_24_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_VTRANS_2_12V_24,
		.port_fault = CHANNELS_FAULT_VTRANS_2_12V_24,
		.gpio_port = VTRANS_2_12V_24_GPIO_Port,
		.gpio_pin = VTRANS_2_12V_24_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_BMSPOWER_PLUG1,
		.port_fault = CHANNELS_FAULT_BMSPOWER_PLUG1,
		.gpio_port = BMSPOWER_PLUG1_GPIO_Port,
		.gpio_pin = BMSPOWER_PLUG1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_BMSPOWER_PLUG2,
		.port_fault = CHANNELS_FAULT_BMSPOWER_PLUG2,
		.gpio_port = BMSPOWER_PLUG2_GPIO_Port,
		.gpio_pin = BMSPOWER_PLUG2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_LED_YELLOW_PLUG1,
		.port_fault = CHANNELS_FAULT_LED_YELLOW_PLUG1,
		.gpio_port = LED_YELLOW_PLUG1_GPIO_Port,
		.gpio_pin = LED_YELLOW_PLUG1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_LED_YELLOW_PLUG2,
		.port_fault = CHANNELS_FAULT_LED_YELLOW_PLUG2,
		.gpio_port = LED_YELLOW_PLUG2_GPIO_Port,
		.gpio_pin = LED_YELLOW_PLUG2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_LED_RED_PLUG1,
		.port_fault = CHANNELS_FAULT_LED_RED_PLUG1,
		.gpio_port = LED_RED_PLUG1_GPIO_Port,
		.gpio_pin = LED_RED_PLUG1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_LED_RED_PLUG2,
		.port_fault = CHANNELS_FAULT_LED_RED_PLUG2,
		.gpio_port = LED_RED_PLUG2_GPIO_Port,
		.gpio_pin = LED_RED_PLUG2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_LED_GREEN_POWER1,
		.port_fault = CHANNELS_FAULT_LED_GREEN_POWER1,
		.gpio_port = LED_GREEN_POWER1_GPIO_Port,
		.gpio_pin = LED_GREEN_POWER1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_LED_GREEN_POWER2,
		.port_fault = CHANNELS_FAULT_LED_GREEN_POWER2,
		.gpio_port = LED_GREEN_POWER2_GPIO_Port,
		.gpio_pin = LED_GREEN_POWER2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_SCLK2,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_SCLK2,
		.gpio_port = CHARGING_LED_SCLK2_GPIO_Port,
		.gpio_pin = CHARGING_LED_SCLK2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_LCLK2,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_LCLK2,
		.gpio_port = CHARGING_LED_LCLK2_GPIO_Port,
		.gpio_pin = CHARGING_LED_LCLK2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_DS2,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_DS2,
		.gpio_port = CHARGING_LED_DS2_GPIO_Port,
		.gpio_pin = CHARGING_LED_DS2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_SCLK1,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_SCLK1,
		.gpio_port = CHARGING_LED_SCLK1_GPIO_Port,
		.gpio_pin = CHARGING_LED_SCLK1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_LCLK1,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_LCLK1,
		.gpio_port = CHARGING_LED_LCLK1_GPIO_Port,
		.gpio_pin = CHARGING_LED_LCLK1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_DS1,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_DS1,
		.gpio_port = CHARGING_LED_DS1_GPIO_Port,
		.gpio_pin = CHARGING_LED_DS1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PAR_EXT_TX1,
		.port_fault = CHANNELS_FAULT_PAR_EXT_TX1,
		.gpio_port = PAR_EXT_TX1_GPIO_Port,
		.gpio_pin = PAR_EXT_TX1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PAR_EXT_TX2,
		.port_fault = CHANNELS_FAULT_PAR_EXT_TX2,
		.gpio_port = PAR_EXT_TX2_GPIO_Port,
		.gpio_pin = PAR_EXT_TX2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PAR_EXT_TX3,
		.port_fault = CHANNELS_FAULT_PAR_EXT_TX3,
		.gpio_port = PAR_EXT_TX3_GPIO_Port,
		.gpio_pin = PAR_EXT_TX3_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PLUG1_LOCK_1,
		.port_fault = CHANNELS_FAULT_PLUG1_LOCK_1,
		.gpio_port = PLUG1_LOCK_1_GPIO_Port,
		.gpio_pin = PLUG1_LOCK_1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PLUG1_LOCK_2,
		.port_fault = CHANNELS_FAULT_PLUG1_LOCK_2,
		.gpio_port = PLUG1_LOCK_2_GPIO_Port,
		.gpio_pin = PLUG1_LOCK_2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PLUG2_LOCK_1,
		.port_fault = CHANNELS_FAULT_PLUG2_LOCK_1,
		.gpio_port = PLUG2_LOCK_1_GPIO_Port,
		.gpio_pin = PLUG2_LOCK_1_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PLUG2_LOCK_2,
		.port_fault = CHANNELS_FAULT_PLUG2_LOCK_2,
		.gpio_port = PLUG2_LOCK_2_GPIO_Port,
		.gpio_pin = PLUG2_LOCK_2_Pin,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
};

static int do_port_output_test(test_ports_output_ctx_t *test_ports_output_ctx, channels_info_t *channels_info)
{
	int ret = 1;

	if(test_ports_output_ctx->test_type_ports == TEST_TYPE_PORTS_NONE) {
		return ret;
	}

	switch(test_ports_output_ctx->state) {
		case 0: {
			HAL_GPIO_WritePin(test_ports_output_ctx->gpio_port, test_ports_output_ctx->gpio_pin, GPIO_PIN_RESET);
			test_ports_output_ctx->stamp = osKernelSysTick();
			test_ports_output_ctx->state = 1;
		}
		break;

		case 1: {
			uint16_t value;

			if(modbus_master_read_items_retry(channels_info->modbus_master_info, 1, test_ports_output_ctx->test_type_ports, 1, &value, 3) == 0) {
				if(value == test_ports_output_ctx->gpio_state1) {
					test_ports_output_ctx->state = 2;
				} else {
					ret = -1;
				}
			} else {
				ret = -1;
			}

			if(ticks_duration(osKernelSysTick(), test_ports_output_ctx->stamp) > 1000) {
				ret = -1;
			}
		}
		break;

		case 2: {
			HAL_GPIO_WritePin(test_ports_output_ctx->gpio_port, test_ports_output_ctx->gpio_pin, GPIO_PIN_SET);
			test_ports_output_ctx->stamp = osKernelSysTick();
			test_ports_output_ctx->state = 3;
		}
		break;

		case 3: {
			uint16_t value;

			if(modbus_master_read_items_retry(channels_info->modbus_master_info, 1, test_ports_output_ctx->test_type_ports, 1, &value, 3) == 0) {
				if(value == test_ports_output_ctx->gpio_state2) {
					ret = 0;
				} else {
					ret = -1;
				}
			} else {
				ret = -1;
			}

			if(ticks_duration(osKernelSysTick(), test_ports_output_ctx->stamp) > 1000) {
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

void handle_next_ports_output_test(test_ports_output_ctx_t *test_ports_output_ctx, channels_info_t *channels_info)
{
	test_port_output_item_t *test_port_output_item;

	if(test_ports_output_ctx->test_type_ports != TEST_TYPE_PORTS_NONE) {
		return;
	}

	if(ARRAY_SIZE(test_port_output_items) <= 0) {
		return;
	}

	test_port_output_item = &test_port_output_items[test_ports_output_ctx->index];

	test_ports_output_ctx->index++;

	if(test_ports_output_ctx->index >= ARRAY_SIZE(test_port_output_items)) {
		test_ports_output_ctx->index = 0;
	}

	test_ports_output_ctx->state = 0;
	test_ports_output_ctx->test_type_ports = test_port_output_item->request_test_type_ports;
	test_ports_output_ctx->port_fault = test_port_output_item->port_fault;
	test_ports_output_ctx->gpio_port = test_port_output_item->gpio_port;
	test_ports_output_ctx->gpio_pin = test_port_output_item->gpio_pin;
	test_ports_output_ctx->gpio_state1 = test_port_output_item->gpio_state1;
	test_ports_output_ctx->gpio_state2 = test_port_output_item->gpio_state2;
}

static void ports_output_test_periodic(test_ports_ctx_t *test_ports_ctx, channels_info_t *channels_info)
{
	test_ports_output_ctx_t *test_ports_output_ctx = &test_ports_ctx->test_ports_output_ctx;
	int ret;

	ret = do_port_output_test(test_ports_output_ctx, channels_info);

	if(ret == -1) {
		set_fault(channels_info->faults, test_ports_output_ctx->port_fault, 1);
		test_ports_output_ctx->test_type_ports = TEST_TYPE_PORTS_NONE;
	} else if(ret == 0) {
		set_fault(channels_info->faults, test_ports_output_ctx->port_fault, 0);
		test_ports_output_ctx->test_type_ports = TEST_TYPE_PORTS_NONE;
	}

	handle_next_ports_output_test(test_ports_output_ctx, channels_info);
}

typedef struct {
	test_type_t request_test_type_ports;
	int port_fault;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t gpio_state1;
	uint8_t gpio_state2;
} test_port_input_item_t;

static test_port_input_item_t test_port_input_items[] = {
	{
		.request_test_type_ports = TEST_TYPE_PORTS_DOOR1_OPEN,
		.port_fault = CHANNELS_FAULT_DOOR1_OPEN,
		.gpio_port = DOOR1_OPEN_GPIO_Port,
		.gpio_pin = DOOR1_OPEN_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_EPO,
		.port_fault = CHANNELS_FAULT_EPO,
		.gpio_port = EPO_GPIO_Port,
		.gpio_pin = EPO_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_FUSE1_OPEN,
		.port_fault = CHANNELS_FAULT_FUSE1_OPEN,
		.gpio_port = FUSE1_OPEN_GPIO_Port,
		.gpio_pin = FUSE1_OPEN_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_FUSE2_OPEN,
		.port_fault = CHANNELS_FAULT_FUSE2_OPEN,
		.gpio_port = FUSE2_OPEN_GPIO_Port,
		.gpio_pin = FUSE2_OPEN_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_MAIN_RLY_PLUG1_BACK,
		.port_fault = CHANNELS_FAULT_MAIN_RLY_PLUG1_BACK,
		.gpio_port = MAIN_RLY_PLUG1_BACK_GPIO_Port,
		.gpio_pin = MAIN_RLY_PLUG1_BACK_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_MAIN_RLY_PLUG2_BACK,
		.port_fault = CHANNELS_FAULT_MAIN_RLY_PLUG2_BACK,
		.gpio_port = MAIN_RLY_PLUG2_BACK_GPIO_Port,
		.gpio_pin = MAIN_RLY_PLUG2_BACK_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_BRG_RLY_DRV_BACK,
		.port_fault = CHANNELS_FAULT_BRG_RLY_DRV_BACK,
		.gpio_port = BRG_RLY_DRV_BACK_GPIO_Port,
		.gpio_pin = BRG_RLY_DRV_BACK_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_1_BACK,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_1_BACK,
		.gpio_port = POWERDISTRIBUTION_1_BACK_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_1_BACK_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_2_BACK,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_2_BACK,
		.gpio_port = POWERDISTRIBUTION_2_BACK_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_2_BACK_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_3_BACK,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_3_BACK,
		.gpio_port = POWERDISTRIBUTION_3_BACK_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_3_BACK_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_4_BACK,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_4_BACK,
		.gpio_port = POWERDISTRIBUTION_4_BACK_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_4_BACK_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_5_BACK,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_5_BACK,
		.gpio_port = POWERDISTRIBUTION_5_BACK_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_5_BACK_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_FAN1_FAULT,
		.port_fault = CHANNELS_FAULT_FAN1_FAULT,
		.gpio_port = FAN1_FAULT_GPIO_Port,
		.gpio_pin = FAN1_FAULT_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_FAN2_FAULT,
		.port_fault = CHANNELS_FAULT_FAN2_FAULT,
		.gpio_port = FAN2_FAULT_GPIO_Port,
		.gpio_pin = FAN2_FAULT_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_FAN3_FAULT,
		.port_fault = CHANNELS_FAULT_FAN3_FAULT,
		.gpio_port = FAN3_FAULT_GPIO_Port,
		.gpio_pin = FAN3_FAULT_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_FAN4_FAULT,
		.port_fault = CHANNELS_FAULT_FAN4_FAULT,
		.gpio_port = FAN4_FAULT_GPIO_Port,
		.gpio_pin = FAN4_FAULT_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_FAN5_FAULT,
		.port_fault = CHANNELS_FAULT_FAN5_FAULT,
		.gpio_port = FAN5_FAULT_GPIO_Port,
		.gpio_pin = FAN5_FAULT_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_FAN5_FAULT,
		.port_fault = CHANNELS_FAULT_FAN5_FAULT,
		.gpio_port = FAN5_FAULT_GPIO_Port,
		.gpio_pin = FAN5_FAULT_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PAR_EXT_RX1,
		.port_fault = CHANNELS_FAULT_PAR_EXT_RX1,
		.gpio_port = PAR_EXT_RX1_GPIO_Port,
		.gpio_pin = PAR_EXT_RX1_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PAR_EXT_RX2,
		.port_fault = CHANNELS_FAULT_PAR_EXT_RX2,
		.gpio_port = PAR_EXT_RX2_GPIO_Port,
		.gpio_pin = PAR_EXT_RX2_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PAR_EXT_RX3,
		.port_fault = CHANNELS_FAULT_PAR_EXT_RX3,
		.gpio_port = PAR_EXT_RX3_GPIO_Port,
		.gpio_pin = PAR_EXT_RX3_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PLUG1_LOCK_BACK,
		.port_fault = CHANNELS_FAULT_PLUG1_LOCK_BACK,
		.gpio_port = PLUG1_LOCK_BACK_GPIO_Port,
		.gpio_pin = PLUG1_LOCK_BACK_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.request_test_type_ports = TEST_TYPE_PORTS_PLUG2_LOCK_BACK,
		.port_fault = CHANNELS_FAULT_PLUG2_LOCK_BACK,
		.gpio_port = PLUG2_LOCK_BACK_GPIO_Port,
		.gpio_pin = PLUG2_LOCK_BACK_Pin,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
};

static int do_port_input_test(test_ports_input_ctx_t *test_ports_input_ctx, channels_info_t *channels_info)
{
	int ret = 1;

	if(test_ports_input_ctx->test_type_ports == TEST_TYPE_PORTS_NONE) {
		return ret;
	}

	switch(test_ports_input_ctx->state) {
		case 0: {
			HAL_GPIO_WritePin(test_ports_input_ctx->gpio_port, test_ports_input_ctx->gpio_pin, GPIO_PIN_RESET);
			test_ports_input_ctx->stamp = osKernelSysTick();
			test_ports_input_ctx->state = 1;
		}
		break;

		case 1: {
			uint16_t value;

			if(modbus_master_read_items_retry(channels_info->modbus_master_info, 1, test_ports_input_ctx->test_type_ports, 1, &value, 3) == 0) {
				if(value == test_ports_input_ctx->gpio_state1) {
					test_ports_input_ctx->state = 2;
				} else {
					ret = -1;
				}
			} else {
				ret = -1;
			}

			if(ticks_duration(osKernelSysTick(), test_ports_input_ctx->stamp) > 1000) {
				ret = -1;
			}
		}
		break;

		case 2: {
			HAL_GPIO_WritePin(test_ports_input_ctx->gpio_port, test_ports_input_ctx->gpio_pin, GPIO_PIN_SET);
			test_ports_input_ctx->stamp = osKernelSysTick();
			test_ports_input_ctx->state = 3;
		}
		break;

		case 3: {
			uint16_t value;

			if(modbus_master_read_items_retry(channels_info->modbus_master_info, 1, test_ports_input_ctx->test_type_ports, 1, &value, 3) == 0) {
				if(value == test_ports_input_ctx->gpio_state2) {
					ret = 0;
				} else {
					ret = -1;
				}
			} else {
				ret = -1;
			}

			if(ticks_duration(osKernelSysTick(), test_ports_input_ctx->stamp) > 1000) {
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

void handle_next_ports_input_test(test_ports_input_ctx_t *test_ports_input_ctx, channels_info_t *channels_info)
{
	test_port_input_item_t *test_port_input_item;

	if(test_ports_input_ctx->test_type_ports != TEST_TYPE_PORTS_NONE) {
		return;
	}

	if(ARRAY_SIZE(test_port_input_items) <= 0) {
		return;
	}

	test_port_input_item = &test_port_input_items[test_ports_input_ctx->index];

	test_ports_input_ctx->index++;

	if(test_ports_input_ctx->index >= ARRAY_SIZE(test_port_input_items)) {
		test_ports_input_ctx->index = 0;
	}

	test_ports_input_ctx->state = 0;
	test_ports_input_ctx->test_type_ports = test_port_input_item->request_test_type_ports;
	test_ports_input_ctx->port_fault = test_port_input_item->port_fault;
	test_ports_input_ctx->gpio_port = test_port_input_item->gpio_port;
	test_ports_input_ctx->gpio_pin = test_port_input_item->gpio_pin;
	test_ports_input_ctx->gpio_state1 = test_port_input_item->gpio_state1;
	test_ports_input_ctx->gpio_state2 = test_port_input_item->gpio_state2;
}

static void ports_input_test_periodic(test_ports_ctx_t *test_ports_ctx, channels_info_t *channels_info)
{
	test_ports_input_ctx_t *test_ports_input_ctx = &test_ports_ctx->test_ports_input_ctx;
	int ret;

	ret = do_port_input_test(test_ports_input_ctx, channels_info);

	if(ret == -1) {
		set_fault(channels_info->faults, test_ports_input_ctx->port_fault, 1);
		test_ports_input_ctx->test_type_ports = TEST_TYPE_PORTS_NONE;
	} else if(ret == 0) {
		set_fault(channels_info->faults, test_ports_input_ctx->port_fault, 0);
		test_ports_input_ctx->test_type_ports = TEST_TYPE_PORTS_NONE;
	}

	handle_next_ports_input_test(test_ports_input_ctx, channels_info);
}


static void ports_test_periodic(void *fn_ctx, void *chain_ctx)
{
	test_ports_ctx_t *test_ports_ctx = (test_ports_ctx_t *)fn_ctx;
	channels_info_t *channels_info = (channels_info_t *)chain_ctx;

	ports_output_test_periodic(test_ports_ctx, channels_info);
	ports_input_test_periodic(test_ports_ctx, channels_info);
}

void start_ports_tests(channels_info_t *channels_info)
{
	test_ports_ctx_t *test_ports_ctx = os_calloc(1, sizeof(test_ports_ctx_t));

	OS_ASSERT(test_ports_ctx != NULL);

	test_ports_ctx->periodic_callback_item.fn = ports_test_periodic;
	test_ports_ctx->periodic_callback_item.fn_ctx = test_ports_ctx;
	OS_ASSERT(register_callback(channels_info->common_periodic_chain, &test_ports_ctx->periodic_callback_item) == 0);
}
