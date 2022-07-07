

/*================================================================
 *
 *
 *   文件名称：channels_addr_handler.c
 *   创 建 者：肖飞
 *   创建日期：2021年07月16日 星期五 14时03分28秒
 *   修改日期：2022年07月07日 星期四 14时06分48秒
 *   描    述：
 *
 *================================================================*/
#include "modbus_data_value.h"
#include "channels.h"
#include "display.h"

#include "log.h"

void channels_modbus_data_action(void *fn_ctx, void *chain_ctx)
{
	channels_info_t *channels_info = (channels_info_t *)fn_ctx;
	modbus_data_ctx_t *modbus_data_ctx = (modbus_data_ctx_t *)chain_ctx;

	//debug("op:%s, addr:%d",
	//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) ? "get" :
	//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) ? "set" :
	//      "unknow",
	//      modbus_data_ctx->addr);

	switch(modbus_data_ctx->addr) {
		case CHANNELS_FAULT_CAN1 ... CHANNELS_FAULT_PLUG2_LOCK_BACK: {
			uint8_t value = get_fault(channels_info->faults, modbus_data_ctx->addr);
			modbus_data_value_r(modbus_data_ctx, value);
		}
		break;

		case 1000: {//测试结果复位
			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) {
				modbus_data_ctx->value = 0;
			} else if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				if(modbus_data_ctx->value != 0) {
					int i;

					for(i = 0; i < CHANNELS_FAULT_SIZE; i++) {
						set_fault(channels_info->faults, i, 1);
					}
				}
			}
		}
		break;

		case 1001 ... 1002: {//插枪状态
			int index = (modbus_data_ctx->addr - 1001) % 2;
			modbus_data_value_r(modbus_data_ctx, channels_info->channel_cc1[index]);
		}
		break;

		case 1003 ... 1004: {//充电电压
			int index = (modbus_data_ctx->addr - 1003) % 2;
			modbus_data_value_r(modbus_data_ctx, channels_info->charger_voltage[index]);
		}
		break;

		case 1005 ... 1006: {//电池电压
			int index = (modbus_data_ctx->addr - 1005) % 2;
			modbus_data_value_r(modbus_data_ctx, channels_info->battery_voltage[index]);
		}
		break;

		case 1007 ... 1010: {//温度
			int index = (modbus_data_ctx->addr - 1007) % 4;
			modbus_data_value_r(modbus_data_ctx, channels_info->temperature[index]);
		}
		break;

		default:
			debug("error! op:%s, addr:%d",
			      (modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) ? "get" :
			      (modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) ? "set" :
			      "unknow",
			      modbus_data_ctx->addr);
			break;
	}
}
