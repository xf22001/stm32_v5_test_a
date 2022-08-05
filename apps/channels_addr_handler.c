

/*================================================================
 *
 *
 *   文件名称：channels_addr_handler.c
 *   创 建 者：肖飞
 *   创建日期：2021年07月16日 星期五 14时03分28秒
 *   修改日期：2022年08月05日 星期五 08时40分24秒
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

		case 1011 ... 1016: {//时间
			static uint16_t local_tm[6] = {0};

			if(modbus_data_ctx->addr == 1011) {
				time_t ts = get_time();
				struct tm tm = *localtime(&ts);

				debug("tm %04d-%02d-%02d %02d:%02d:%02d",
				      tm.tm_year + 1900,
				      tm.tm_mon + 1,
				      tm.tm_mday,
				      tm.tm_hour,
				      tm.tm_min,
				      tm.tm_sec);
				local_tm[0] = tm.tm_year + 1900;
				local_tm[1] = tm.tm_mon + 1;
				local_tm[2] = tm.tm_mday;
				local_tm[3] = tm.tm_hour;
				local_tm[4] = tm.tm_min;
				local_tm[5] = tm.tm_sec;
			}

			modbus_data_buffer_rw(modbus_data_ctx, local_tm, 6 * 2, modbus_data_ctx->addr - 1011);
		}
		break;

		case 1017 ... 1018: {
			int index = (modbus_data_ctx->addr - 1017) % 2;
			modbus_data_value_r(modbus_data_ctx, channels_info->insulation[index]);
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
