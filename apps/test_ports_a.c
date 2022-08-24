

/*================================================================
 *
 *
 *   文件名称：test_ports_a.c
 *   创 建 者：肖飞
 *   创建日期：2022年05月16日 星期一 16时36分32秒
 *   修改日期：2022年08月24日 星期三 11时35分04秒
 *   描    述：
 *
 *================================================================*/
#include "test_ports_a.h"
#include "main.h"
#include "test_type.h"
#include "hw_adc.h"
#include "pt_temperature.h"

#include "log.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

typedef struct {
	int index;
	uint8_t state;
	uint32_t stamp;

	void *item;
} test_ports_output_ctx_t;

typedef struct {
	int index;
	uint8_t state;
	uint32_t stamp;

	void *item;
} test_ports_input_ctx_t;

typedef struct {
	int index;

	void *item;
} test_ports_cc1_ctx_t;

typedef struct {
	int index;

	void *item;
} test_ports_voltage_ctx_t;

typedef struct {
	int index;

	void *item;
} test_ports_temperature_ctx_t;

typedef struct {
	int index;
	uint8_t state;
	uint32_t stamp;

	uint16_t insulation_detect_u1_ad;
	uint16_t insulation_detect_u2_ad;
	float insulation_detect_u1;
	float insulation_detect_u2;
	float insulation_resistor_rx;
	float insulation_resistor_ry;

	void *item;
} test_ports_insulation_ctx_t;

typedef struct {
	callback_item_t periodic_callback_item;
	test_ports_output_ctx_t test_ports_output_ctx;
	test_ports_input_ctx_t test_ports_input_ctx;
	test_ports_cc1_ctx_t test_ports_cc1_ctx;
	test_ports_voltage_ctx_t test_ports_voltage_ctx;
	test_ports_temperature_ctx_t test_ports_temperature_ctx;
	test_ports_insulation_ctx_t test_ports_insulation_ctx;
} test_ports_ctx_t;

typedef struct {
	test_type_t test_type_ports;
	int port_fault;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t default_state;
	uint8_t test_state1;
	uint8_t test_state2;
	uint8_t gpio_state1;
	uint8_t gpio_state2;
} test_port_output_item_t;

static test_port_output_item_t test_port_output_items[] = {
	{
		.test_type_ports = TEST_TYPE_PORTS_CONTACTOR_DRV,
		.port_fault = CHANNELS_FAULT_CONTACTOR_DRV,
		.gpio_port = CONTACTOR_DRV_GPIO_Port,
		.gpio_pin = CONTACTOR_DRV_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_FAN1_RLY_DRV,
		.port_fault = CHANNELS_FAULT_FAN1_RLY_DRV,
		.gpio_port = FAN1_RLY_DRV_GPIO_Port,
		.gpio_pin = FAN1_RLY_DRV_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_VTRANS_1_12V_24,
		.port_fault = CHANNELS_FAULT_VTRANS_1_12V_24,
		.gpio_port = VTRANS_1_12V_24_GPIO_Port,
		.gpio_pin = VTRANS_1_12V_24_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_VTRANS_2_12V_24,
		.port_fault = CHANNELS_FAULT_VTRANS_2_12V_24,
		.gpio_port = VTRANS_2_12V_24_GPIO_Port,
		.gpio_pin = VTRANS_2_12V_24_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_BMSPOWER_PLUG1,
		.port_fault = CHANNELS_FAULT_BMSPOWER_PLUG1,
		.gpio_port = BMSPOWER_PLUG1_GPIO_Port,
		.gpio_pin = BMSPOWER_PLUG1_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_BMSPOWER_PLUG2,
		.port_fault = CHANNELS_FAULT_BMSPOWER_PLUG2,
		.gpio_port = BMSPOWER_PLUG2_GPIO_Port,
		.gpio_pin = BMSPOWER_PLUG2_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_LED_YELLOW_PLUG1,
		.port_fault = CHANNELS_FAULT_LED_YELLOW_PLUG1,
		.gpio_port = LED_YELLOW_PLUG1_GPIO_Port,
		.gpio_pin = LED_YELLOW_PLUG1_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_LED_YELLOW_PLUG2,
		.port_fault = CHANNELS_FAULT_LED_YELLOW_PLUG2,
		.gpio_port = LED_YELLOW_PLUG2_GPIO_Port,
		.gpio_pin = LED_YELLOW_PLUG2_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_LED_RED_PLUG1,
		.port_fault = CHANNELS_FAULT_LED_RED_PLUG1,
		.gpio_port = LED_RED_PLUG1_GPIO_Port,
		.gpio_pin = LED_RED_PLUG1_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_LED_RED_PLUG2,
		.port_fault = CHANNELS_FAULT_LED_RED_PLUG2,
		.gpio_port = LED_RED_PLUG2_GPIO_Port,
		.gpio_pin = LED_RED_PLUG2_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_LED_GREEN_POWER1,
		.port_fault = CHANNELS_FAULT_LED_GREEN_POWER1,
		.gpio_port = LED_GREEN_POWER1_GPIO_Port,
		.gpio_pin = LED_GREEN_POWER1_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_LED_GREEN_POWER2,
		.port_fault = CHANNELS_FAULT_LED_GREEN_POWER2,
		.gpio_port = LED_GREEN_POWER2_GPIO_Port,
		.gpio_pin = LED_GREEN_POWER2_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_SCLK2,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_SCLK2,
		.gpio_port = CHARGING_LED_SCLK2_GPIO_Port,
		.gpio_pin = CHARGING_LED_SCLK2_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_LCLK2,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_LCLK2,
		.gpio_port = CHARGING_LED_LCLK2_GPIO_Port,
		.gpio_pin = CHARGING_LED_LCLK2_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_DS2,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_DS2,
		.gpio_port = CHARGING_LED_DS2_GPIO_Port,
		.gpio_pin = CHARGING_LED_DS2_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_SCLK1,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_SCLK1,
		.gpio_port = CHARGING_LED_SCLK1_GPIO_Port,
		.gpio_pin = CHARGING_LED_SCLK1_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_LCLK1,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_LCLK1,
		.gpio_port = CHARGING_LED_LCLK1_GPIO_Port,
		.gpio_pin = CHARGING_LED_LCLK1_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_CHARGING_LED_DS1,
		.port_fault = CHANNELS_FAULT_CHARGING_LED_DS1,
		.gpio_port = CHARGING_LED_DS1_GPIO_Port,
		.gpio_pin = CHARGING_LED_DS1_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_PAR_EXT_TX1,
		.port_fault = CHANNELS_FAULT_PAR_EXT_TX1,
		.gpio_port = PAR_EXT_TX1_GPIO_Port,
		.gpio_pin = PAR_EXT_TX1_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_PAR_EXT_TX2,
		.port_fault = CHANNELS_FAULT_PAR_EXT_TX2,
		.gpio_port = PAR_EXT_TX2_GPIO_Port,
		.gpio_pin = PAR_EXT_TX2_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_PAR_EXT_TX3,
		.port_fault = CHANNELS_FAULT_PAR_EXT_TX3,
		.gpio_port = PAR_EXT_TX3_GPIO_Port,
		.gpio_pin = PAR_EXT_TX3_Pin,
		.default_state = GPIO_PIN_SET,
		.test_state1 = GPIO_PIN_RESET,
		.test_state2 = GPIO_PIN_SET,
		.gpio_state1 = GPIO_PIN_RESET,
		.gpio_state2 = GPIO_PIN_SET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_PLUG1_LOCK_1,
		.port_fault = CHANNELS_FAULT_PLUG1_LOCK_1,
		.gpio_port = PLUG1_LOCK_1_GPIO_Port,
		.gpio_pin = PLUG1_LOCK_1_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_PLUG1_LOCK_2,
		.port_fault = CHANNELS_FAULT_PLUG1_LOCK_2,
		.gpio_port = PLUG1_LOCK_2_GPIO_Port,
		.gpio_pin = PLUG1_LOCK_2_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_PLUG2_LOCK_1,
		.port_fault = CHANNELS_FAULT_PLUG2_LOCK_1,
		.gpio_port = PLUG2_LOCK_1_GPIO_Port,
		.gpio_pin = PLUG2_LOCK_1_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_PLUG2_LOCK_2,
		.port_fault = CHANNELS_FAULT_PLUG2_LOCK_2,
		.gpio_port = PLUG2_LOCK_2_GPIO_Port,
		.gpio_pin = PLUG2_LOCK_2_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_DISCHARGE_RLY_PLUG1,
		.port_fault = CHANNELS_FAULT_DISCHARGE_RLY_PLUG1,
		.gpio_port = DISCHARGE_RLY_PLUG1_GPIO_Port,
		.gpio_pin = DISCHARGE_RLY_PLUG1_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_DISCHARGE_RLY_PLUG2,
		.port_fault = CHANNELS_FAULT_DISCHARGE_RLY_PLUG2,
		.gpio_port = DISCHARGE_RLY_PLUG2_GPIO_Port,
		.gpio_pin = DISCHARGE_RLY_PLUG2_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_MARQUEE1,
		.port_fault = CHANNELS_FAULT_MARQUEE1,
		.gpio_port = MARQUEE1__GPIO_Port,
		.gpio_pin = MARQUEE1__Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_MARQUEE2,
		.port_fault = CHANNELS_FAULT_MARQUEE2,
		.gpio_port = MARQUEE2__GPIO_Port,
		.gpio_pin = MARQUEE2__Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_MAIN_RLY_PLUG1,
		.port_fault = CHANNELS_FAULT_MAIN_RLY_PLUG1,
		.gpio_port = MAIN_RLY_PLUG1_GPIO_Port,
		.gpio_pin = MAIN_RLY_PLUG1_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_MAIN_RLY_PLUG2,
		.port_fault = CHANNELS_FAULT_MAIN_RLY_PLUG2,
		.gpio_port = MAIN_RLY_PLUG2_GPIO_Port,
		.gpio_pin = MAIN_RLY_PLUG2_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_BRG_RLY_DRV,
		.port_fault = CHANNELS_FAULT_BRG_RLY_DRV,
		.gpio_port = BRG_RLY_DRV_GPIO_Port,
		.gpio_pin = BRG_RLY_DRV_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_1,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_1,
		.gpio_port = POWERDISTRIBUTION_1_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_1_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_2,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_2,
		.gpio_port = POWERDISTRIBUTION_2_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_2_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_3,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_3,
		.gpio_port = POWERDISTRIBUTION_3_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_3_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_4,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_4,
		.gpio_port = POWERDISTRIBUTION_4_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_4_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_5,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_5,
		.gpio_port = POWERDISTRIBUTION_5_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_5_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
};

static int do_port_output_test(test_ports_output_ctx_t *test_ports_output_ctx, channels_info_t *channels_info)
{
	int ret = 1;

	test_port_output_item_t *item = (test_port_output_item_t *)test_ports_output_ctx->item;

	if(item == NULL) {
		return ret;
	}

	if(item->test_type_ports == TEST_TYPE_PORTS_NONE) {
		return ret;
	}

	if(get_fault(channels_info->faults, item->port_fault) == 0) {
		ret = 0;
		return ret;
	}

	switch(test_ports_output_ctx->state) {
		case 0: {
			HAL_GPIO_WritePin(item->gpio_port, item->gpio_pin, item->test_state1);
			test_ports_output_ctx->stamp = osKernelSysTick();
			test_ports_output_ctx->state = 1;
		}
		break;

		case 1: {
			uint16_t value;
			uint8_t fault = 0;

			if(modbus_master_read_items_retry(channels_info->modbus_master_info, 1, item->test_type_ports, 1, &value, 3) == 0) {
				if(value == item->gpio_state1) {
					test_ports_output_ctx->state = 2;
				} else {
					//debug("test %d state %d value:%d, gpio_state1:%d", item->port_fault, test_ports_output_ctx->state, value, item->gpio_state1);
				}
			} else {
				fault = 1;
				debug("");
				ret = -1;
			}

			set_fault(channels_info->faults, CHANNELS_FAULT_UART5, fault);

			if(ticks_duration(osKernelSysTick(), test_ports_output_ctx->stamp) > 1000) {
				debug("test %d state %d value:%d, gpio_state1:%d", item->port_fault, test_ports_output_ctx->state, value, item->gpio_state1);
				ret = -1;
			}
		}
		break;

		case 2: {
			HAL_GPIO_WritePin(item->gpio_port, item->gpio_pin, item->test_state2);
			test_ports_output_ctx->stamp = osKernelSysTick();
			test_ports_output_ctx->state = 3;
		}
		break;

		case 3: {
			uint16_t value;
			uint8_t fault = 0;

			if(modbus_master_read_items_retry(channels_info->modbus_master_info, 1, item->test_type_ports, 1, &value, 3) == 0) {
				if(value == item->gpio_state2) {
					ret = 0;
				} else {
					//debug("test %d state %d value:%d, gpio_state2:%d", item->port_fault, test_ports_output_ctx->state, value, item->gpio_state2);
				}
			} else {
				fault = 1;
				debug("");
				ret = -1;
			}

			set_fault(channels_info->faults, CHANNELS_FAULT_UART5, fault);

			if(ticks_duration(osKernelSysTick(), test_ports_output_ctx->stamp) > 1000) {
				debug("test %d state %d value:%d, gpio_state2:%d", item->port_fault, test_ports_output_ctx->state, value, item->gpio_state2);
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

	if(test_ports_output_ctx->item != NULL) {
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

	//debug("start output test %d", test_port_output_item->port_fault);
	test_ports_output_ctx->item = test_port_output_item;
}

static void ports_output_test_periodic(test_ports_ctx_t *test_ports_ctx, channels_info_t *channels_info)
{
	test_ports_output_ctx_t *test_ports_output_ctx = &test_ports_ctx->test_ports_output_ctx;
	test_port_output_item_t *item = (test_port_output_item_t *)test_ports_output_ctx->item;
	int ret;

	ret = do_port_output_test(test_ports_output_ctx, channels_info);

	if(ret == -1) {
		if(item != NULL) {
			set_fault(channels_info->faults, item->port_fault, 1);
			HAL_GPIO_WritePin(item->gpio_port, item->gpio_pin, item->default_state);
		}

		test_ports_output_ctx->item = NULL;
	} else if(ret == 0) {
		if(item != NULL) {
			set_fault(channels_info->faults, item->port_fault, 0);
			HAL_GPIO_WritePin(item->gpio_port, item->gpio_pin, item->default_state);
		}

		test_ports_output_ctx->item = NULL;
	}

	handle_next_ports_output_test(test_ports_output_ctx, channels_info);
}

typedef struct {
	test_type_t test_type_ports;
	int port_fault;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t default_state;
	uint8_t test_state1;
	uint8_t test_state2;
	uint8_t gpio_state1;
	uint8_t gpio_state2;
} test_port_input_item_t;

static test_port_input_item_t test_port_input_items[] = {
	{
		.test_type_ports = TEST_TYPE_PORTS_DOOR1_OPEN,
		.port_fault = CHANNELS_FAULT_DOOR1_OPEN,
		.gpio_port = DOOR1_OPEN_GPIO_Port,
		.gpio_pin = DOOR1_OPEN_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_EPO,
		.port_fault = CHANNELS_FAULT_EPO,
		.gpio_port = EPO_GPIO_Port,
		.gpio_pin = EPO_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_FUSE1_OPEN,
		.port_fault = CHANNELS_FAULT_FUSE1_OPEN,
		.gpio_port = FUSE1_OPEN_GPIO_Port,
		.gpio_pin = FUSE1_OPEN_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_FUSE2_OPEN,
		.port_fault = CHANNELS_FAULT_FUSE2_OPEN,
		.gpio_port = FUSE2_OPEN_GPIO_Port,
		.gpio_pin = FUSE2_OPEN_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_MAIN_RLY_PLUG1_BACK,
		.port_fault = CHANNELS_FAULT_MAIN_RLY_PLUG1_BACK,
		.gpio_port = MAIN_RLY_PLUG1_BACK_GPIO_Port,
		.gpio_pin = MAIN_RLY_PLUG1_BACK_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_MAIN_RLY_PLUG2_BACK,
		.port_fault = CHANNELS_FAULT_MAIN_RLY_PLUG2_BACK,
		.gpio_port = MAIN_RLY_PLUG2_BACK_GPIO_Port,
		.gpio_pin = MAIN_RLY_PLUG2_BACK_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_BRG_RLY_DRV_BACK,
		.port_fault = CHANNELS_FAULT_BRG_RLY_DRV_BACK,
		.gpio_port = BRG_RLY_DRV_BACK_GPIO_Port,
		.gpio_pin = BRG_RLY_DRV_BACK_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_1_BACK,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_1_BACK,
		.gpio_port = POWERDISTRIBUTION_1_BACK_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_1_BACK_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_2_BACK,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_2_BACK,
		.gpio_port = POWERDISTRIBUTION_2_BACK_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_2_BACK_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_3_BACK,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_3_BACK,
		.gpio_port = POWERDISTRIBUTION_3_BACK_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_3_BACK_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_4_BACK,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_4_BACK,
		.gpio_port = POWERDISTRIBUTION_4_BACK_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_4_BACK_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_POWERDISTRIBUTION_5_BACK,
		.port_fault = CHANNELS_FAULT_POWERDISTRIBUTION_5_BACK,
		.gpio_port = POWERDISTRIBUTION_5_BACK_GPIO_Port,
		.gpio_pin = POWERDISTRIBUTION_5_BACK_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_FAN1_FAULT,
		.port_fault = CHANNELS_FAULT_FAN1_FAULT,
		.gpio_port = FAN1_FAULT_GPIO_Port,
		.gpio_pin = FAN1_FAULT_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_FAN2_FAULT,
		.port_fault = CHANNELS_FAULT_FAN2_FAULT,
		.gpio_port = FAN2_FAULT_GPIO_Port,
		.gpio_pin = FAN2_FAULT_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_FAN3_FAULT,
		.port_fault = CHANNELS_FAULT_FAN3_FAULT,
		.gpio_port = FAN3_FAULT_GPIO_Port,
		.gpio_pin = FAN3_FAULT_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_FAN4_FAULT,
		.port_fault = CHANNELS_FAULT_FAN4_FAULT,
		.gpio_port = FAN4_FAULT_GPIO_Port,
		.gpio_pin = FAN4_FAULT_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_FAN5_FAULT,
		.port_fault = CHANNELS_FAULT_FAN5_FAULT,
		.gpio_port = FAN5_FAULT_GPIO_Port,
		.gpio_pin = FAN5_FAULT_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	//无供电
	//{
	//	.test_type_ports = TEST_TYPE_PORTS_PAR_EXT_RX1,
	//	.port_fault = CHANNELS_FAULT_PAR_EXT_RX1,
	//	.gpio_port = PAR_EXT_RX1_GPIO_Port,
	//	.gpio_pin = PAR_EXT_RX1_Pin,
	//	.default_state = GPIO_PIN_SET,
	//	.test_state1 = GPIO_PIN_RESET,
	//	.test_state2 = GPIO_PIN_SET,
	//	.gpio_state1 = GPIO_PIN_RESET,
	//	.gpio_state2 = GPIO_PIN_SET,
	//},
	//{
	//	.test_type_ports = TEST_TYPE_PORTS_PAR_EXT_RX2,
	//	.port_fault = CHANNELS_FAULT_PAR_EXT_RX2,
	//	.gpio_port = PAR_EXT_RX2_GPIO_Port,
	//	.gpio_pin = PAR_EXT_RX2_Pin,
	//	.default_state = GPIO_PIN_SET,
	//	.test_state1 = GPIO_PIN_RESET,
	//	.test_state2 = GPIO_PIN_SET,
	//	.gpio_state1 = GPIO_PIN_RESET,
	//	.gpio_state2 = GPIO_PIN_SET,
	//},
	//{
	//	.test_type_ports = TEST_TYPE_PORTS_PAR_EXT_RX3,
	//	.port_fault = CHANNELS_FAULT_PAR_EXT_RX3,
	//	.gpio_port = PAR_EXT_RX3_GPIO_Port,
	//	.gpio_pin = PAR_EXT_RX3_Pin,
	//	.default_state = GPIO_PIN_SET,
	//	.test_state1 = GPIO_PIN_RESET,
	//	.test_state2 = GPIO_PIN_SET,
	//	.gpio_state1 = GPIO_PIN_RESET,
	//	.gpio_state2 = GPIO_PIN_SET,
	//},
	{
		.test_type_ports = TEST_TYPE_PORTS_PLUG1_LOCK_BACK,
		.port_fault = CHANNELS_FAULT_PLUG1_LOCK_BACK,
		.gpio_port = PLUG1_LOCK_BACK_GPIO_Port,
		.gpio_pin = PLUG1_LOCK_BACK_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_PLUG2_LOCK_BACK,
		.port_fault = CHANNELS_FAULT_PLUG2_LOCK_BACK,
		.gpio_port = PLUG2_LOCK_BACK_GPIO_Port,
		.gpio_pin = PLUG2_LOCK_BACK_Pin,
		.default_state = GPIO_PIN_RESET,
		.test_state1 = GPIO_PIN_SET,
		.test_state2 = GPIO_PIN_RESET,
		.gpio_state1 = GPIO_PIN_SET,
		.gpio_state2 = GPIO_PIN_RESET,
	},
};

static int do_port_input_test(test_ports_input_ctx_t *test_ports_input_ctx, channels_info_t *channels_info)
{
	int ret = 1;
	test_port_input_item_t *item = (test_port_input_item_t *)test_ports_input_ctx->item;

	if(item == NULL) {
		return ret;
	}

	if(item->test_type_ports == TEST_TYPE_PORTS_NONE) {
		return ret;
	}

	if(get_fault(channels_info->faults, item->port_fault) == 0) {
		ret = 0;
		return ret;
	}

	switch(test_ports_input_ctx->state) {
		case 0: {
			uint8_t fault = 0;

			if(modbus_master_write_one_item_retry(channels_info->modbus_master_info, 1, item->test_type_ports, item->test_state1, 10) == 0) {
				test_ports_input_ctx->state = 1;
			} else {
				fault = 1;
				debug("");
				ret = -1;
			}

			set_fault(channels_info->faults, CHANNELS_FAULT_UART5, fault);
			test_ports_input_ctx->stamp = osKernelSysTick();
		}
		break;

		case 1: {
			if(HAL_GPIO_ReadPin(item->gpio_port, item->gpio_pin) == item->gpio_state1) {
				test_ports_input_ctx->state = 2;
			} else {
			}

			if(ticks_duration(osKernelSysTick(), test_ports_input_ctx->stamp) > 1000) {
				debug("test %d state %d value:%d, gpio_state1:%d", item->port_fault, test_ports_input_ctx->state, HAL_GPIO_ReadPin(item->gpio_port, item->gpio_pin), item->gpio_state1);
				ret = -1;
			}
		}
		break;

		case 2: {
			uint8_t fault = 0;

			if(modbus_master_write_one_item_retry(channels_info->modbus_master_info, 1, item->test_type_ports, item->test_state2, 10) == 0) {
				test_ports_input_ctx->state = 3;
			} else {
				fault = 1;
				debug("");
				ret = -1;
			}

			set_fault(channels_info->faults, CHANNELS_FAULT_UART5, fault);
			test_ports_input_ctx->stamp = osKernelSysTick();
		}
		break;

		case 3: {
			if(HAL_GPIO_ReadPin(item->gpio_port, item->gpio_pin) == item->gpio_state2) {
				ret = 0;
			} else {
			}

			if(ticks_duration(osKernelSysTick(), test_ports_input_ctx->stamp) > 1000) {
				debug("test %d state %d value:%d, gpio_state2:%d", item->port_fault, test_ports_input_ctx->state, HAL_GPIO_ReadPin(item->gpio_port, item->gpio_pin), item->gpio_state2);
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

	if(test_ports_input_ctx->item != NULL) {
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
	//debug("start input test %d", test_port_input_item->port_fault);
	test_ports_input_ctx->item = test_port_input_item;
}

static void ports_input_test_periodic(test_ports_ctx_t *test_ports_ctx, channels_info_t *channels_info)
{
	test_ports_input_ctx_t *test_ports_input_ctx = &test_ports_ctx->test_ports_input_ctx;
	test_port_input_item_t *item = (test_port_input_item_t *)test_ports_input_ctx->item;
	int ret;

	ret = do_port_input_test(test_ports_input_ctx, channels_info);

	if(ret == -1) {
		if(item != NULL) {
			set_fault(channels_info->faults, item->port_fault, 1);

			while(modbus_master_write_one_item_retry(channels_info->modbus_master_info, 1, item->test_type_ports, item->default_state, 3) != 0) {
				debug("");
			}
		}

		test_ports_input_ctx->item = NULL;
	} else if(ret == 0) {
		if(item != NULL) {
			set_fault(channels_info->faults, item->port_fault, 0);

			while(modbus_master_write_one_item_retry(channels_info->modbus_master_info, 1, item->test_type_ports, item->default_state, 3) != 0) {
				debug("");
			}
		}

		test_ports_input_ctx->item = NULL;
	}

	handle_next_ports_input_test(test_ports_input_ctx, channels_info);
}

/* 11.2 - 12.8
 * 5.2 - 6.8
 * 3.2 - 4.8
 */
#define CP_AD_VOLTAGE_CONNECT_OFF_UP 1280
#define CP_AD_VOLTAGE_CONNECT_OFF_DOWN 1120
#define CP_AD_VOLTAGE_CONNECT_ON_LOCK_OFF_UP 680
#define CP_AD_VOLTAGE_CONNECT_ON_LOCK_OFF_DOWN 520
#define CP_AD_VOLTAGE_CONNECT_ON_LOCK_ON_UP 480
#define CP_AD_VOLTAGE_CONNECT_ON_LOCK_ON_DOWN 320

typedef enum {
	CP_AD_CONNECT_TYPE_INVALID = 0,
	CP_AD_CONNECT_TYPE_CONNECT_OFF,
	CP_AD_CONNECT_TYPE_CONNECT_ON_LOCK_OFF,
	CP_AD_CONNECT_TYPE_CONNECT_ON,
} cp_ad_connect_type_t;

typedef struct {
	test_type_t test_type_ports;
	void *cp_ad_adc;
	uint8_t cp_ad_adc_rank;
} channel_cc1_adc_item_t;

static channel_cc1_adc_item_t channel_cc1_adc_items[] = {
	{
		.test_type_ports = TEST_TYPE_PORTS_CHANNEL0_CC1,
		.cp_ad_adc = &hadc1,
		.cp_ad_adc_rank = 0,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_CHANNEL1_CC1,
		.cp_ad_adc = &hadc1,
		.cp_ad_adc_rank = 1,
	},
};

static uint8_t do_ports_cc1_test(test_ports_cc1_ctx_t *test_ports_cc1_ctx, channels_info_t *channels_info)
{
	channel_cc1_adc_item_t *item = (channel_cc1_adc_item_t *)test_ports_cc1_ctx->item;
	adc_info_t *adc_info = NULL;
	uint8_t charger_connect_type;//cp_ad_connect_type_t
	uint8_t charger_connect_state = 0;
	uint16_t cp_ad;
	uint16_t cp_ad_voltage;

	if(item == NULL) {
		return charger_connect_state;
	}

	adc_info = get_or_alloc_adc_info(item->cp_ad_adc);

	cp_ad = get_adc_value(adc_info, item->cp_ad_adc_rank);

	//debug("channel %d cp ad:%d", test_ports_cc1_ctx->index, cp_ad);

	cp_ad_voltage = cp_ad * 3300 / 4096;//0v-1.2v 采样 0v-12v

	//(V - 0.5) * 2 / 102 * 8 * 4 / 3 = u
	//V - 0.5 = u / (2 / 102 * 8 * 4 / 3)
	//修正前
	//V = u / (2 / 102 * 8 * 4 / 3) + 0.5
	//修正后
	//V = u / (1.8667 / 101.8667 * 8 * 4 / 3) + 0.5

	cp_ad_voltage = cp_ad_voltage * 5.1159817458616805 / 10 + 50;

	//debug("channel %d cp ad voltage:%d", channel_info->channel_id, cp_ad_voltage);

	if(cp_ad_voltage > CP_AD_VOLTAGE_CONNECT_OFF_UP) {
		charger_connect_type = CP_AD_CONNECT_TYPE_INVALID;
		charger_connect_state = 0;
	} else if(cp_ad_voltage >= CP_AD_VOLTAGE_CONNECT_OFF_DOWN) {
		charger_connect_type = CP_AD_CONNECT_TYPE_CONNECT_OFF;
		charger_connect_state = 0;
	} else if(cp_ad_voltage > CP_AD_VOLTAGE_CONNECT_ON_LOCK_OFF_UP) {
		charger_connect_type = CP_AD_CONNECT_TYPE_INVALID;
		charger_connect_state = 0;
	} else if(cp_ad_voltage >= CP_AD_VOLTAGE_CONNECT_ON_LOCK_OFF_DOWN) {
		charger_connect_type = CP_AD_CONNECT_TYPE_CONNECT_ON_LOCK_OFF;
		charger_connect_state = 0;
	} else if(cp_ad_voltage > CP_AD_VOLTAGE_CONNECT_ON_LOCK_ON_UP) {
		charger_connect_type = CP_AD_CONNECT_TYPE_INVALID;
		charger_connect_state = 0;
	} else if(cp_ad_voltage >= CP_AD_VOLTAGE_CONNECT_ON_LOCK_ON_DOWN) {
		charger_connect_type = CP_AD_CONNECT_TYPE_CONNECT_ON;
		charger_connect_state = 1;
	} else {
		charger_connect_type = CP_AD_CONNECT_TYPE_INVALID;
		charger_connect_state = 0;
	}

	charger_connect_type = charger_connect_type;

	return charger_connect_state;
}

void handle_next_ports_cc1_test(test_ports_cc1_ctx_t *test_ports_cc1_ctx, channels_info_t *channels_info)
{
	channel_cc1_adc_item_t *item = (channel_cc1_adc_item_t *)test_ports_cc1_ctx->item;
	channel_cc1_adc_item_t *channel_cc1_adc_item;

	if(item != NULL) {
		return;
	}

	if(ARRAY_SIZE(channel_cc1_adc_items) <= 0) {
		return;
	}

	channel_cc1_adc_item = &channel_cc1_adc_items[test_ports_cc1_ctx->index];

	test_ports_cc1_ctx->index++;

	if(test_ports_cc1_ctx->index >= ARRAY_SIZE(channel_cc1_adc_items)) {
		test_ports_cc1_ctx->index = 0;
	}

	test_ports_cc1_ctx->item = channel_cc1_adc_item;
}

static void ports_cc1_test_periodic(test_ports_ctx_t *test_ports_ctx, channels_info_t *channels_info)
{
	test_ports_cc1_ctx_t *test_ports_cc1_ctx = &test_ports_ctx->test_ports_cc1_ctx;
	channel_cc1_adc_item_t *item = (channel_cc1_adc_item_t *)test_ports_cc1_ctx->item;
	uint8_t ret;

	ret = do_ports_cc1_test(test_ports_cc1_ctx, channels_info);

	if(item != NULL) {
		switch(item->test_type_ports) {
			case TEST_TYPE_PORTS_CHANNEL0_CC1 ... TEST_TYPE_PORTS_CHANNEL1_CC1: {
				channels_info->channel_cc1[item->test_type_ports - TEST_TYPE_PORTS_CHANNEL0_CC1] = ret;
			}
			break;

			default: {
			}
			break;
		}
	}

	test_ports_cc1_ctx->item = NULL;

	handle_next_ports_cc1_test(test_ports_cc1_ctx, channels_info);
}

typedef struct {
	test_type_t test_type_ports;
	void *charge_voltage_adc;
	uint8_t charge_voltage_adc_rank;
	void *battery_voltage_adc;
	uint8_t battery_voltage_adc_rank;
} channel_voltage_adc_item_t;

static channel_voltage_adc_item_t channel_voltage_adc_items[] = {
	{
		.test_type_ports = TEST_TYPE_PORTS_CHANNEL0_VOLTAGE,
		.charge_voltage_adc = &hadc3,
		.charge_voltage_adc_rank = 2,
		.battery_voltage_adc = &hadc3,
		.battery_voltage_adc_rank = 10,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_CHANNEL1_VOLTAGE,
		.charge_voltage_adc = &hadc3,
		.charge_voltage_adc_rank = 6,
		.battery_voltage_adc = &hadc3,
		.battery_voltage_adc_rank = 7,
	},
};

typedef struct {
	uint16_t charger_voltage;//0.1v
	uint16_t battery_voltage;//0.1v
} voltage_info_t;

static voltage_info_t do_ports_voltage_test(test_ports_voltage_ctx_t *test_ports_voltage_ctx, channels_info_t *channels_info)
{
	channel_voltage_adc_item_t *item = (channel_voltage_adc_item_t *)test_ports_voltage_ctx->item;
	adc_info_t *charge_voltage_adc_info;
	adc_info_t *battery_voltage_adc_info;
	uint16_t charge_voltage_ad = 0;
	uint16_t battery_voltage_ad = 0;
	float charge_voltage = 0.0;
	float battery_voltage = 0.0;
	voltage_info_t voltage_info = {0};

	if(item == NULL) {
		return voltage_info;
	}

	charge_voltage_adc_info = get_or_alloc_adc_info(item->charge_voltage_adc);
	charge_voltage_ad = get_adc_value(charge_voltage_adc_info,
	                                  item->charge_voltage_adc_rank);
	//debug("channel %d charge voltage ad:%d", test_ports_voltage_ctx->index, charge_voltage_ad);
	charge_voltage = charge_voltage_ad * 3.3 / 4096;
	//debug("channel %d charge voltage:%f", test_ports_voltage_ctx->index, charge_voltage);
	charge_voltage = charge_voltage * 499 * 7;
	//debug("channel %d charge voltage:%f", test_ports_voltage_ctx->index, charge_voltage);
	voltage_info.charger_voltage = charge_voltage;
	//debug("channel %d charge voltage:%d", test_ports_voltage_ctx->index, voltage_info.charger_voltage);

	battery_voltage_adc_info = get_or_alloc_adc_info(item->battery_voltage_adc);
	battery_voltage_ad = get_adc_value(battery_voltage_adc_info,
	                                   item->battery_voltage_adc_rank);
	//debug("channel %d battery voltage ad:%d", test_ports_voltage_ctx->index, battery_voltage_ad);
	battery_voltage = battery_voltage_ad * 3.3 / 4096;
	//debug("channel %d battery voltage:%f", test_ports_voltage_ctx->index, battery_voltage);
	battery_voltage = (battery_voltage - 1.49) * 2 * 3493;
	//debug("channel %d battery voltage:%f", test_ports_voltage_ctx->index, battery_voltage);
	voltage_info.battery_voltage = battery_voltage;
	//debug("channel %d battery voltage:%d", test_ports_voltage_ctx->index, voltage_info.battery_voltage);

	return voltage_info;
}

void handle_next_ports_voltage_test(test_ports_voltage_ctx_t *test_ports_voltage_ctx, channels_info_t *channels_info)
{
	channel_voltage_adc_item_t *item = (channel_voltage_adc_item_t *)test_ports_voltage_ctx->item;
	channel_voltage_adc_item_t *channel_voltage_adc_item;

	if(item != NULL) {
		return;
	}

	if(ARRAY_SIZE(channel_voltage_adc_items) <= 0) {
		return;
	}

	channel_voltage_adc_item = &channel_voltage_adc_items[test_ports_voltage_ctx->index];

	test_ports_voltage_ctx->index++;

	if(test_ports_voltage_ctx->index >= ARRAY_SIZE(channel_voltage_adc_items)) {
		test_ports_voltage_ctx->index = 0;
	}

	test_ports_voltage_ctx->item = channel_voltage_adc_item;
}

static void ports_voltage_test_periodic(test_ports_ctx_t *test_ports_ctx, channels_info_t *channels_info)
{
	test_ports_voltage_ctx_t *test_ports_voltage_ctx = &test_ports_ctx->test_ports_voltage_ctx;
	channel_voltage_adc_item_t *item = (channel_voltage_adc_item_t *)test_ports_voltage_ctx->item;
	voltage_info_t voltage_info = {0};

	voltage_info = do_ports_voltage_test(test_ports_voltage_ctx, channels_info);

	if(item != NULL) {
		switch(item->test_type_ports) {
			case TEST_TYPE_PORTS_CHANNEL0_VOLTAGE ... TEST_TYPE_PORTS_CHANNEL1_VOLTAGE: {
				channels_info->charger_voltage[item->test_type_ports - TEST_TYPE_PORTS_CHANNEL0_VOLTAGE] = voltage_info.charger_voltage;
				channels_info->battery_voltage[item->test_type_ports - TEST_TYPE_PORTS_CHANNEL0_VOLTAGE] = voltage_info.battery_voltage;
			}
			break;

			default: {
			}
			break;
		}
	}

	test_ports_voltage_ctx->item = NULL;

	handle_next_ports_voltage_test(test_ports_voltage_ctx, channels_info);
}

typedef struct {
	test_type_t test_type_ports;
	void *temperature_adc;
	uint8_t temperature_adc_rank;
} temperature_adc_item_t;

static temperature_adc_item_t temperature_adc_items[] = {
	{
		.test_type_ports = TEST_TYPE_PORTS_TEMPERATURE_1,
		.temperature_adc = &hadc3,
		.temperature_adc_rank = 5,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_TEMPERATURE_2,
		.temperature_adc = &hadc3,
		.temperature_adc_rank = 9,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_TEMPERATURE_3,
		.temperature_adc = &hadc3,
		.temperature_adc_rank = 1,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_TEMPERATURE_4,
		.temperature_adc = &hadc3,
		.temperature_adc_rank = 0,
	},
};

static int16_t do_ports_temperature_test(test_ports_temperature_ctx_t *test_ports_temperature_ctx, channels_info_t *channels_info)
{
	temperature_adc_item_t *item = (temperature_adc_item_t *)test_ports_temperature_ctx->item;
	adc_info_t *temperature_adc_info;
	uint16_t temperature_ad = 0;
	int16_t temperature = 0;

	if(item == NULL) {
		return temperature;
	}

	temperature_adc_info = get_or_alloc_adc_info(item->temperature_adc);
	temperature_ad = get_adc_value(temperature_adc_info,
	                               item->temperature_adc_rank);

	temperature = get_pt_temperature(1000, temperature_ad, 4095);

	return temperature;
}

void handle_next_ports_temperature_test(test_ports_temperature_ctx_t *test_ports_temperature_ctx, channels_info_t *channels_info)
{
	temperature_adc_item_t *item = (temperature_adc_item_t *)test_ports_temperature_ctx->item;
	temperature_adc_item_t *temperature_adc_item;

	if(item != NULL) {
		return;
	}

	if(ARRAY_SIZE(temperature_adc_items) <= 0) {
		return;
	}

	temperature_adc_item = &temperature_adc_items[test_ports_temperature_ctx->index];

	test_ports_temperature_ctx->index++;

	if(test_ports_temperature_ctx->index >= ARRAY_SIZE(temperature_adc_items)) {
		test_ports_temperature_ctx->index = 0;
	}

	test_ports_temperature_ctx->item = temperature_adc_item;
}

static void ports_temperature_test_periodic(test_ports_ctx_t *test_ports_ctx, channels_info_t *channels_info)
{
	test_ports_temperature_ctx_t *test_ports_temperature_ctx = &test_ports_ctx->test_ports_temperature_ctx;
	temperature_adc_item_t *item = (temperature_adc_item_t *)test_ports_temperature_ctx->item;
	int16_t temperature = 0;

	temperature = do_ports_temperature_test(test_ports_temperature_ctx, channels_info);

	if(item != NULL) {
		switch(item->test_type_ports) {
			case TEST_TYPE_PORTS_TEMPERATURE_1 ... TEST_TYPE_PORTS_TEMPERATURE_4: {
				channels_info->temperature[item->test_type_ports - TEST_TYPE_PORTS_TEMPERATURE_1] = temperature;
			}
			break;

			default: {
			}
			break;
		}
	}

	test_ports_temperature_ctx->item = NULL;

	handle_next_ports_temperature_test(test_ports_temperature_ctx, channels_info);
}

typedef struct {
	test_type_t test_type_ports;

	void *insulation_voltage_adc;
	uint8_t insulation_voltage_adc_rank;
	void *insulation_k1_gpio;
	uint16_t insulation_k1_pin;
	void *insulation_k3_gpio;
	uint16_t insulation_k3_pin;
} insulation_adc_item_t;

static insulation_adc_item_t insulation_adc_items[] = {
	{
		.test_type_ports = TEST_TYPE_PORTS_INSULATION_1,
		.insulation_voltage_adc = &hadc3,
		.insulation_voltage_adc_rank = 3,
		.insulation_k1_gpio = K1_PLUG1_GPIO_Port,
		.insulation_k1_pin = K1_PLUG1_Pin,
		.insulation_k3_gpio = K3_PLUG1_GPIO_Port,
		.insulation_k3_pin = K3_PLUG1_Pin,
	},
	{
		.test_type_ports = TEST_TYPE_PORTS_INSULATION_2,
		.insulation_voltage_adc = &hadc3,
		.insulation_voltage_adc_rank = 4,
		.insulation_k1_gpio = K1_PLUG2_GPIO_Port,
		.insulation_k1_pin = K1_PLUG2_Pin,
		.insulation_k3_gpio = K3_PLUG2_GPIO_Port,
		.insulation_k3_pin = K3_PLUG2_Pin,
	},
};

static int do_ports_insulation_test(test_ports_insulation_ctx_t *test_ports_insulation_ctx, channels_info_t *channels_info, uint8_t *insulation, uint16_t *insulation_voltage)
{
	insulation_adc_item_t *item = (insulation_adc_item_t *)test_ports_insulation_ctx->item;
	int ret = 1;

	if(item == NULL) {
		return ret;
	}

	if(item->test_type_ports == TEST_TYPE_PORTS_NONE) {
		return ret;
	}

	switch(test_ports_insulation_ctx->state) {
		case 0: {
			test_ports_insulation_ctx->state = 1;
		}
		break;

		case 1: {//闭合k1
			HAL_GPIO_WritePin(item->insulation_k1_gpio,
			                  item->insulation_k1_pin,
			                  GPIO_PIN_SET);
			test_ports_insulation_ctx->stamp = osKernelSysTick();
			test_ports_insulation_ctx->state = 2;
		}
		break;

		case 2: {//k1稳定1秒
			if(ticks_duration(osKernelSysTick(), test_ports_insulation_ctx->stamp) >= 1000) {
				test_ports_insulation_ctx->state = 3;
			}
		}
		break;

		case 3: {//读u1电压
			adc_info_t *adc_info = get_or_alloc_adc_info(item->insulation_voltage_adc);

			test_ports_insulation_ctx->insulation_detect_u1_ad = get_adc_value(adc_info,
			        item->insulation_voltage_adc_rank);
			test_ports_insulation_ctx->insulation_detect_u1 = test_ports_insulation_ctx->insulation_detect_u1_ad * 3.3 / 4096;
			debug("test type %d insulation_detect_u1:%f", item->test_type_ports, test_ports_insulation_ctx->insulation_detect_u1);
			*insulation_voltage = test_ports_insulation_ctx->insulation_detect_u1 * 10000;
			ret = 2;
			test_ports_insulation_ctx->state = 4;
		}
		break;

		case 4: {//闭合k3
			HAL_GPIO_WritePin(item->insulation_k3_gpio,
			                  item->insulation_k3_pin,
			                  GPIO_PIN_SET);
			test_ports_insulation_ctx->stamp = osKernelSysTick();
			test_ports_insulation_ctx->state = 5;
		}
		break;

		case 5: {//k3稳定1秒
			if(ticks_duration(osKernelSysTick(), test_ports_insulation_ctx->stamp) >= 1000) {
				test_ports_insulation_ctx->state = 6;
			}
		}
		break;

		case 6: {//读u2电压
			adc_info_t *adc_info = get_or_alloc_adc_info(item->insulation_voltage_adc);

			test_ports_insulation_ctx->insulation_detect_u2_ad = get_adc_value(adc_info,
			        item->insulation_voltage_adc_rank);
			test_ports_insulation_ctx->insulation_detect_u2 = test_ports_insulation_ctx->insulation_detect_u2_ad * 3.3 / 4096;
			debug("test type %d insulation_detect_u2:%f", item->test_type_ports, test_ports_insulation_ctx->insulation_detect_u2);
			*insulation_voltage = test_ports_insulation_ctx->insulation_detect_u2 * 10000;
			ret = 2;
			test_ports_insulation_ctx->state = 7;
		}
		break;

		case 7: {//断开k1,k3
			HAL_GPIO_WritePin(item->insulation_k3_gpio,
			                  item->insulation_k3_pin,
			                  GPIO_PIN_RESET);
			HAL_GPIO_WritePin(item->insulation_k1_gpio,
			                  item->insulation_k1_pin,
			                  GPIO_PIN_RESET);
			test_ports_insulation_ctx->state = 8;
		}
		break;

		case 8: {
			uint32_t v_dc = 330;
			float u1 = test_ports_insulation_ctx->insulation_detect_u1;
			float u2 = test_ports_insulation_ctx->insulation_detect_u2;
			float ryy;
			float rx_resistor = 0;//0.1M
			float ry_resistor = 0;//0.1M

			debug("test type %d v_dc:%d", item->test_type_ports, v_dc);

			test_ports_insulation_ctx->insulation_resistor_rx = (0.00325 * v_dc * (u1 - u2)) / (u1 * u2 - 0.00325 * v_dc * (u1 - u2));
			debug("test type %d insulation_resistor_rx:%f", item->test_type_ports, test_ports_insulation_ctx->insulation_resistor_rx);

			if(test_ports_insulation_ctx->insulation_resistor_rx <= 0) {
				debug("");
			}

			ryy = (u1 * (0.00325 * v_dc - u2)) / (u2 * (0.00325 * v_dc - u1)) - 1;
			debug("test type %d ryy:%f", item->test_type_ports, ryy);

			test_ports_insulation_ctx->insulation_resistor_ry = (ryy * 24) / (24 - ryy);
			debug("test type %d insulation_resistor_ry:%f", item->test_type_ports, test_ports_insulation_ctx->insulation_resistor_ry);

			if(test_ports_insulation_ctx->insulation_resistor_ry <= 0) {
				debug("");
			}

			rx_resistor = test_ports_insulation_ctx->insulation_resistor_rx * 10;
			debug("test type %d rx_resistor:%d", item->test_type_ports, (int)rx_resistor);
			ry_resistor = test_ports_insulation_ctx->insulation_resistor_ry * 10;
			debug("test type %d ry_resistor:%d", item->test_type_ports, (int)ry_resistor);
			*insulation = ry_resistor < rx_resistor ? ry_resistor : rx_resistor;
			test_ports_insulation_ctx->state = 9;
			ret = 0;
		}
		break;

		default: {
		}
		break;
	}

	return ret;
}

void handle_next_ports_insulation_test(test_ports_insulation_ctx_t *test_ports_insulation_ctx, channels_info_t *channels_info)
{
	insulation_adc_item_t *item = (insulation_adc_item_t *)test_ports_insulation_ctx->item;
	insulation_adc_item_t *insulation_adc_item;

	if(item != NULL) {
		return;
	}

	if(ARRAY_SIZE(insulation_adc_items) <= 0) {
		return;
	}

	insulation_adc_item = &insulation_adc_items[test_ports_insulation_ctx->index];

	test_ports_insulation_ctx->index++;

	if(test_ports_insulation_ctx->index >= ARRAY_SIZE(insulation_adc_items)) {
		test_ports_insulation_ctx->index = 0;
	}

	test_ports_insulation_ctx->item = insulation_adc_item;
	test_ports_insulation_ctx->state = 0;
}

static void ports_insulation_test_periodic(test_ports_ctx_t *test_ports_ctx, channels_info_t *channels_info)
{
	test_ports_insulation_ctx_t *test_ports_insulation_ctx = &test_ports_ctx->test_ports_insulation_ctx;
	insulation_adc_item_t *item = (insulation_adc_item_t *)test_ports_insulation_ctx->item;
	int ret;
	uint8_t insulation;
	uint16_t insulation_voltage;

	ret = do_ports_insulation_test(test_ports_insulation_ctx, channels_info, &insulation, &insulation_voltage);

	if(item != NULL) {
		if(ret != 1) {
			switch(item->test_type_ports) {
				case TEST_TYPE_PORTS_INSULATION_1 ... TEST_TYPE_PORTS_INSULATION_2: {
					if(ret == 0) {
						channels_info->insulation[item->test_type_ports - TEST_TYPE_PORTS_INSULATION_1] = insulation;
					} else if(ret == 2) {
						channels_info->insulation_voltage[item->test_type_ports - TEST_TYPE_PORTS_INSULATION_1] = insulation_voltage;
					}
				}
				break;

				default: {
				}
				break;
			}

			if(ret == 0) {
				test_ports_insulation_ctx->item = NULL;
			}
		}
	}


	handle_next_ports_insulation_test(test_ports_insulation_ctx, channels_info);
}

static void ports_test_periodic(void *fn_ctx, void *chain_ctx)
{
	test_ports_ctx_t *test_ports_ctx = (test_ports_ctx_t *)fn_ctx;
	channels_info_t *channels_info = (channels_info_t *)chain_ctx;

	ports_output_test_periodic(test_ports_ctx, channels_info);
	ports_input_test_periodic(test_ports_ctx, channels_info);

	ports_cc1_test_periodic(test_ports_ctx, channels_info);
	ports_voltage_test_periodic(test_ports_ctx, channels_info);
	ports_temperature_test_periodic(test_ports_ctx, channels_info);
	ports_insulation_test_periodic(test_ports_ctx, channels_info);
}

void start_ports_tests(channels_info_t *channels_info)
{
	test_ports_ctx_t *test_ports_ctx = os_calloc(1, sizeof(test_ports_ctx_t));
	int i;

	OS_ASSERT(test_ports_ctx != NULL);

	for(i = 0; i < ARRAY_SIZE(test_port_output_items); i++) {
		test_port_output_item_t *item = &test_port_output_items[i];
		debug("test %d value:%d", item->port_fault, item->default_state);
		HAL_GPIO_WritePin(item->gpio_port, item->gpio_pin, item->default_state);
		set_fault(channels_info->faults, item->port_fault, 1);
	}

	for(i = 0; i < ARRAY_SIZE(test_port_input_items); i++) {
		test_port_input_item_t *item = &test_port_input_items[i];
		debug("test %d value:%d", item->port_fault, item->default_state);

		if(modbus_master_write_one_item_retry(channels_info->modbus_master_info, 1, item->test_type_ports, item->default_state, 3) != 0) {
			debug("");
		}

		set_fault(channels_info->faults, item->port_fault, 1);
	}

	test_ports_ctx->periodic_callback_item.fn = ports_test_periodic;
	test_ports_ctx->periodic_callback_item.fn_ctx = test_ports_ctx;
	OS_ASSERT(register_callback(channels_info->common_periodic_chain, &test_ports_ctx->periodic_callback_item) == 0);
}
