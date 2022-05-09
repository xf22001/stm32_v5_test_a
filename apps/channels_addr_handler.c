

/*================================================================
 *
 *
 *   文件名称：channels_addr_handler.c
 *   创 建 者：肖飞
 *   创建日期：2021年07月16日 星期五 14时03分28秒
 *   修改日期：2022年03月25日 星期五 09时39分04秒
 *   描    述：
 *
 *================================================================*/
#include "modbus_data_value.h"
#include "app.h"
#include "channels.h"
#include "charger.h"
#include "display.h"
#include "power_manager.h"
#include "net_client.h"

#include "log.h"

void channels_modbus_data_action(void *fn_ctx, void *chain_ctx)
{
	channels_info_t *channels_info = (channels_info_t *)fn_ctx;
	channels_settings_t *channels_settings = &channels_info->channels_settings;
	modbus_data_ctx_t *modbus_data_ctx = (modbus_data_ctx_t *)chain_ctx;

	//debug("op:%s, addr:%d",
	//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) ? "get" :
	//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) ? "set" :
	//      "unknow",
	//      modbus_data_ctx->addr);

	switch(modbus_data_ctx->addr) {
		case 0: {//模块个数
			power_manager_info_t *power_manager_info = (power_manager_info_t *)channels_info->power_manager_info;
			uint8_t power_module_number = power_manager_info->power_modules_info->power_module_number;
			modbus_data_value_r(modbus_data_ctx, power_module_number);
		}
		break;

		case 1: {//模块最大输出电压 0.1V
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_max_output_voltage);
		}
		break;

		case 2: {//模块最小输出电压 0.1V
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_min_output_voltage);
		}
		break;

		case 3: {//模块最大输出电流 0.1A
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_max_output_current);
		}
		break;

		case 4: {//模块最小输出电流 0.1A
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_min_output_current);
		}
		break;

		case 5: {//输入过压阈值 0.1V
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_max_input_voltage);
		}
		break;

		case 6: {//输入欠压阈值 0.1V
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_min_input_voltage);
		}
		break;

		case 7: {//输出过压阈值 0.1V
		}
		break;

		case 8: {//输出过流阈值 0.1a
		}
		break;

		case 9: {//刷卡设置 (0:不刷卡 1:刷卡 )
			modbus_data_value_rw(modbus_data_ctx, channels_settings->authorize);
		}
		break;

		case 10: {//枪充电模式 (0:单枪充电  1:双枪充电 2: 两枪一车 3:四枪)
			uint8_t mode = 1;
			modbus_data_value_rw(modbus_data_ctx, mode);
		}
		break;

		case 11: {//宇通修改电流偏移量模式 (0:未使能   1:使能)
			uint8_t mode = 0;
			modbus_data_value_rw(modbus_data_ctx, mode);
		}
		break;

		case 12: {//直流表交流表选择
		}
		break;

		case 13: {//后台设置
			app_info_t *app_info = get_app_info();
			modbus_data_value_rw(modbus_data_ctx, app_info->display_cache_app.request_type);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				app_info->display_cache_app.request_type_sync = 1;
			}
		}
		break;

		case 14: {//辅助电源模式
		}
		break;

		case 15: {//0:同时充 1:轮流充
		}
		break;

		case 16: {//CP AD 采样
		}
		break;

		case 17: {//电表倍率
		}
		break;

		case 18: {//空载保护(0:保护 1:不保护)
			modbus_data_value_rw(modbus_data_ctx, channels_settings->noload_protect_disable);
		}
		break;

		case 19: {//SOC值，设置。达到此SOC值，充电桩会保护停机
			modbus_data_value_rw(modbus_data_ctx, channels_settings->soc_threshold);
		}
		break;

		case 20 ... 307: {//小时(bcd) 分钟(bcd) 电价(0.0001元) 电价(0.0001元) 服务费(0.0001元) 服务费(0.0001元)
			modbus_data_buffer_rw(modbus_data_ctx, channels_info->display_cache_channels.price_item_cache, 6 * 48 * 2, modbus_data_ctx->addr - 20);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				channels_info->display_cache_channels.price_sync = 1;
			}
		}
		break;

		case 308 ... 312: {//ASCII码  IP 250
			app_info_t *app_info = get_app_info();
			modbus_data_buffer_rw(modbus_data_ctx, app_info->display_cache_app.ip, 5 * 2, modbus_data_ctx->addr - 308);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				app_info->display_cache_app.ip_sync = 1;
			}
		}
		break;

		case 313: {//1号堆模块个数
			power_manager_settings_t *power_manager_settings = &channels_settings->power_manager_settings;
			power_manager_group_settings_t *power_manager_group_settings = &power_manager_settings->power_manager_group_settings[0];
			power_module_group_settings_t *power_module_group_settings = &power_manager_group_settings->power_module_group_settings[0];
			modbus_data_value_rw(modbus_data_ctx, power_module_group_settings->power_module_number);
		}
		break;

		case 314: {//2号堆模块个数
			power_manager_settings_t *power_manager_settings = &channels_settings->power_manager_settings;
			power_manager_group_settings_t *power_manager_group_settings = &power_manager_settings->power_manager_group_settings[0];
			power_module_group_settings_t *power_module_group_settings = &power_manager_group_settings->power_module_group_settings[1];
			modbus_data_value_rw(modbus_data_ctx, power_module_group_settings->power_module_number);
		}
		break;

		case 315: {//3号堆模块个数
			power_manager_settings_t *power_manager_settings = &channels_settings->power_manager_settings;
			power_manager_group_settings_t *power_manager_group_settings = &power_manager_settings->power_manager_group_settings[0];
			power_module_group_settings_t *power_module_group_settings = &power_manager_group_settings->power_module_group_settings[2];
			modbus_data_value_rw(modbus_data_ctx, power_module_group_settings->power_module_number);
		}
		break;

		case 316: {//4号堆模块个数
			power_manager_settings_t *power_manager_settings = &channels_settings->power_manager_settings;
			power_manager_group_settings_t *power_manager_group_settings = &power_manager_settings->power_manager_group_settings[0];
			power_module_group_settings_t *power_module_group_settings = &power_manager_group_settings->power_module_group_settings[3];
			modbus_data_value_rw(modbus_data_ctx, power_module_group_settings->power_module_number);
		}
		break;

		case 317 ... 332: {//后台使用的域名
		}
		break;

		case 333: {//自动充电 1:打开  0:关闭
		}
		break;

		case 334: {//电压电流数据源 0:电表采样 1:模块采样
		}
		break;

		case 335: {//预充使能
			modbus_data_value_rw(modbus_data_ctx, channels_settings->precharge_enable);
		}
		break;

		case 336: {//电价精度设置 0：两位   1：四位
		}
		break;

		case 337 ... 342: {//电表A表号
			channel_info_t *channel_info = channels_info->channel_info + 0;
			modbus_data_buffer_rw(modbus_data_ctx, channel_info->display_cache_channel.dlt_645_addr, 6 * 2, modbus_data_ctx->addr - 337);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				channel_info->display_cache_channel.dlt_645_addr_sync = 1;
			}
		}
		break;

		case 343 ... 348: {//电表B表号
			channel_info_t *channel_info = channels_info->channel_info + 1;
			modbus_data_buffer_rw(modbus_data_ctx, channel_info->display_cache_channel.dlt_645_addr, 6 * 2, modbus_data_ctx->addr - 337);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				channel_info->display_cache_channel.dlt_645_addr_sync = 1;
			}
		}
		break;

		case 349 ... 354: {//电表C表号
			channel_info_t *channel_info = channels_info->channel_info + 2;
			modbus_data_buffer_rw(modbus_data_ctx, channel_info->display_cache_channel.dlt_645_addr, 6 * 2, modbus_data_ctx->addr - 337);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				channel_info->display_cache_channel.dlt_645_addr_sync = 1;
			}
		}
		break;

		case 355 ... 360: {//电表D表号
			channel_info_t *channel_info = channels_info->channel_info + 3;
			modbus_data_buffer_rw(modbus_data_ctx, channel_info->display_cache_channel.dlt_645_addr, 6 * 2, modbus_data_ctx->addr - 337);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				channel_info->display_cache_channel.dlt_645_addr_sync = 1;
			}
		}
		break;

		case 361 ... 376: {//ASCII码  设备ID
			app_info_t *app_info = get_app_info();
			modbus_data_buffer_rw(modbus_data_ctx, app_info->display_cache_app.device_id, 16 * 2, modbus_data_ctx->addr - 361);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				app_info->display_cache_app.device_id_sync = 1;
			}
		}
		break;

		case 377 ... 380: {//静态ip设置
			app_info_t *app_info = get_app_info();
			modbus_data_buffer_rw(modbus_data_ctx, app_info->display_cache_app.ip, 4 * 2, modbus_data_ctx->addr - 377);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				app_info->display_cache_app.ip_sync = 1;
			}
		}
		break;

		case 381 ... 384: {//子网掩码
			app_info_t *app_info = get_app_info();
			modbus_data_buffer_rw(modbus_data_ctx, app_info->display_cache_app.sn, 4 * 2, modbus_data_ctx->addr - 377);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				app_info->display_cache_app.ip_sync = 1;
			}
		}
		break;

		case 385 ... 388: {//网关
			app_info_t *app_info = get_app_info();
			modbus_data_buffer_rw(modbus_data_ctx, app_info->display_cache_app.gw, 4 * 2, modbus_data_ctx->addr - 377);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				app_info->display_cache_app.ip_sync = 1;
			}
		}
		break;

		case 389: {//ip获取设置
			app_info_t *app_info = get_app_info();
			modbus_data_value_rw(modbus_data_ctx, app_info->mechine_info.dhcp_enable);
		}
		break;

		case 390 ... 395: {//Mac地址设置
		}
		break;

		case 396: {//mac地址获取设置
		}
		break;

		case 397: {//整机最大输出功率 0.1kW
			modbus_data_value_rw(modbus_data_ctx, channels_settings->power_threshold);
		}
		break;

		case 398: {//电源模块类型      (0:英可瑞协议    1:华为协议  2:国网协议 3:英飞源协议)
			modbus_data_value_rw(modbus_data_ctx, channels_settings->power_module_settings.power_module_type);
		}
		break;

		case 399: {//华为模块基准参考电流 (0.1A)
			modbus_data_value_rw(modbus_data_ctx, channels_settings->power_module_settings.rate_current);
		}
		break;

		case 400: {//双桩一车模式  (0:正常模式  1：双桩一车模式)
		}
		break;

		case 401: {//电量设置 (0:2位小数   1:3位小数)
		}
		break;

		case 2000: {//主控板版本号
			uint16_t major_min = get_u16_from_u8_lh(get_bcd_from_u8(VER_MINOR), get_bcd_from_u8(VER_MAJOR));
			debug("major_min:%04x", major_min);
			modbus_data_value_r(modbus_data_ctx, major_min);
		}
		break;

		case 2001: {//主控板扩展版本号
			uint8_t rev_b0 = get_bcd_from_u8(VER_MINOR);
			uint8_t rev_b1 = get_bcd_from_u8(VER_MAJOR);
			uint16_t rev = get_u16_from_u8_lh(rev_b0, rev_b1);
			debug("rev:%04x", rev);
			modbus_data_value_r(modbus_data_ctx, rev);
		}
		break;

		case 2002 ... 2008: {//BCD码 系统时间 秒 分 时 日 月 年 星期
			app_info_t *app_info = get_app_info();

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) {
				time_t ts = get_time();
				struct tm tm = *localtime(&ts);
				u_uint16_bytes_t u_uint16_bytes;

				app_info->display_cache_app.time[0] = get_bcd_from_u8(tm.tm_sec);
				app_info->display_cache_app.time[1] = get_bcd_from_u8(tm.tm_min);
				app_info->display_cache_app.time[2] = get_bcd_from_u8(tm.tm_hour);
				app_info->display_cache_app.time[3] = get_bcd_from_u8(tm.tm_mday);
				app_info->display_cache_app.time[4] = get_bcd_from_u8(tm.tm_mon + 1);
				u_uint16_bytes.v = tm.tm_year + 1900;
				app_info->display_cache_app.time[5] = get_u16_from_u8_lh(get_bcd_from_u8(u_uint16_bytes.s.byte0), get_bcd_from_u8(u_uint16_bytes.s.byte1));
				app_info->display_cache_app.time[6] = get_bcd_from_u8(tm.tm_wday + 1);
			}

			modbus_data_buffer_rw(modbus_data_ctx, app_info->display_cache_app.time, 7 * 2, modbus_data_ctx->addr - 2002);

		}
		break;

		case 2009: {//BCD码 时间更新标志
		}
		break;

		case 2010 ... 2015: {//BCD码 设置时间 秒 分 时 日 月 年
			app_info_t *app_info = get_app_info();
			modbus_data_buffer_rw(modbus_data_ctx, app_info->display_cache_app.set_time, 6 * 2, modbus_data_ctx->addr - 2010);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				app_info->display_cache_app.set_time_sync = 1;
			}
		}
		break;

		case 2016 ... 2031: {//ASCII  密码输入区
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			//net_client_info_t *net_client_info = get_net_client_info();
			//account_request_info_t account_request_info = {0};
			modbus_data_buffer_rw(modbus_data_ctx, channel_info->display_cache_channel.password, 16 * 2, modbus_data_ctx->addr - 2016);

			//OS_ASSERT(channels_info->display_cache_channels.current_channel < channels_info->channel_number);

			//todo 待确认
			//if(net_client_info == NULL) {
			//	account_request_info.account_type = ACCOUNT_TYPE_CARD;
			//	account_request_info.card_id = card_reader_data->id;
			//	account_request_info.password = (char *)channel_info->display_cache_channel.password;
			//	account_request_info.channel_info = channel_info;
			//	account_request_info.fn = account_request_cb;
			//	net_client_net_client_ctrl_cmd(net_client_info, NET_CLIENT_CTRL_CMD_QUERY_ACCOUNT, &account_request_info);
			//} else {
			//	//无后台刷卡
			//}
		}
		break;

		case 2032: {//网络通信状态
			int state = 0;
			net_client_info_t *net_client_info = get_net_client_info();

			if(get_client_state(net_client_info) == CLIENT_CONNECTED) {
				state = 1;
			}

			modbus_data_value_r(modbus_data_ctx, state);
		}
		break;

		case 2033: {//刷卡板连接状态 0：未连接 1：连接
			uint16_t status = 1;

			if(get_fault(channels_info->faults, CHANNELS_FAULT_CARD_READER) == 1) {
				status = 0;
			}

			modbus_data_value_r(modbus_data_ctx, status);
		}
		break;

		case 2034: {//测试模式
		}
		break;

		case 2035: {//A相输入电压 0.1V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->va);
		}
		break;

		case 2036: {//B相输入电压 0.1V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->vb);
		}
		break;

		case 2037: {//C相输入电压 0.1V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->vc);
		}
		break;

		case 2038: {//系统运行时间 1分钟
			uint32_t up_time = osKernelSysTick() / 60000;
			modbus_data_value_r(modbus_data_ctx, up_time);
		}
		break;

		case 2039: {//弹窗提示地址
		}
		break;

		case 2040: {//系统故障状态
			int fault = 0;

			//channels faults
			if(get_first_fault(channels_info->faults) != -1) {
				fault = 1;
			}

			modbus_data_value_r(modbus_data_ctx, fault);
		}
		break;

		case 2041 ... 2044: {//系统故障状态 按位表示
		}
		break;

		case 2045: {//控制板温度
			modbus_data_value_r(modbus_data_ctx, channels_info->temperature);
		}
		break;

		case 2046: {//接触器粘连状态
		}
		break;

		case 2047: {//最小剩余栈空间 1 byte
		}
		break;

		case 2048: {//剩余堆空间 1 byte
		}
		break;

		case 2500: {//当前记录条数
			channel_record_task_info_t *channel_record_task_info = get_or_alloc_channel_record_task_info(0);
			uint16_t end = channel_record_task_info->channel_record_info.end;
			uint16_t start = channel_record_task_info->channel_record_info.start;
			uint16_t value;

			if(end < start) {
				end += CHANNEL_RECORD_NUMBER;
			}

			value = end - start;
			modbus_data_value_r(modbus_data_ctx, value);
		}
		break;

		case 2501 ... 2503: {//检索年 月 日
			modbus_data_buffer_rw(modbus_data_ctx, &channels_info->display_cache_channels.record_dt_cache, 3 * 2, modbus_data_ctx->addr - 2501);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				channels_info->display_cache_channels.record_sync = 1;
			}
		}
		break;

		case 2504: {//查询确认
			modbus_data_value_rw(modbus_data_ctx, channels_info->display_cache_channels.record_load_cmd);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				channels_info->display_cache_channels.record_sync = 1;
			}
		}
		break;

		case 2505: {//未上传记录数
			channel_record_task_info_t *channel_record_task_info = get_or_alloc_channel_record_task_info(0);
			modbus_data_value_r(modbus_data_ctx, channel_record_task_info->finish_state_count);
		}
		break;

		case 2506: {//查询尖峰平谷电量
		}
		break;

		case 2550 ... 3049: {//充电记录数据
			modbus_data_buffer_rw(modbus_data_ctx, channels_info->display_cache_channels.record_item_cache, 50 * 10 * 2, modbus_data_ctx->addr - 2550);
		}
		break;

		case 3050 ... 3149: {//模块数据
			modbus_data_buffer_rw(modbus_data_ctx, channels_info->display_cache_channels.module_item_cache, 10 * 10 * 2, modbus_data_ctx->addr - 3050);
		}
		break;

		case 3150 ... 3349: {
		}
		break;

		case 3500: {//充电状态 (0:待机  1:准备就绪  2:正在充电)
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->state);
		}
		break;

		case 3501: {//充电电压 0.1V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->voltage);
		}
		break;

		case 3502: {//充电电流 0.1A
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->current);
		}
		break;

		case 3503: {//充电电量 0.001度
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, get_u16_1_from_u32(channel_info->channel_record_item.charge_energy));
		}
		break;

		case 3504: {//充电电量 0.001度
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, get_u16_0_from_u32(channel_info->channel_record_item.charge_energy));
		}
		break;

		case 3505: {//充电时间 1分
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->channel_record_item.charge_duration);
		}
		break;

		case 3506: {//BMS需求电压 0.1V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->require_voltage);
		}
		break;

		case 3507: {//BMS需求电流 0.1A
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->require_current);
		}
		break;

		case 3508: {//预付款 0.01元
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, get_u16_1_from_u32(channel_info->channel_record_item.withholding));
		}
		break;

		case 3509: {//预付款 0.01元
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, get_u16_0_from_u32(channel_info->channel_record_item.withholding));
		}
		break;

		case 3510 ... 3525: {//账户ID
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_buffer_rw(modbus_data_ctx, channel_info->channel_record_item.account, 16 * 2, modbus_data_ctx->addr - 3510);
		}
		break;

		case 3526: {//账户余额 0.01元
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, get_u16_1_from_u32(channel_info->channel_record_item.account_balance));
		}
		break;

		case 3527: {//账户余额 0.01元
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, get_u16_0_from_u32(channel_info->channel_record_item.account_balance));
		}
		break;

		case 3528: {//消费金额 0.01元
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, get_u16_1_from_u32(channel_info->channel_record_item.amount / 1000000));
		}
		break;

		case 3529: {//消费金额 0.01元
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, get_u16_0_from_u32(channel_info->channel_record_item.amount / 1000000));
		}
		break;

		case 3530: {//当前电价 0.0001元
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			price_info_t *price_info_energy = &channels_info->channels_settings.price_info_energy;
			uint8_t index = get_seg_index_by_ts(get_time());
			uint32_t price = price_info_energy->price[index];
			modbus_data_value_r(modbus_data_ctx, get_u16_1_from_u32(price));
		}
		break;

		case 3531: {//当前电价 0.0001元
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			price_info_t *price_info_energy = &channels_info->channels_settings.price_info_energy;
			uint8_t index = get_seg_index_by_ts(get_time());
			uint32_t price = price_info_energy->price[index];
			modbus_data_value_r(modbus_data_ctx, get_u16_0_from_u32(price));
		}
		break;

		case 3532: {//服务费率 0.0001元
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			price_info_t *price_info_service = &channels_info->channels_settings.price_info_service;
			uint8_t index = get_seg_index_by_ts(get_time());
			uint32_t price = price_info_service->price[index];
			modbus_data_value_r(modbus_data_ctx, get_u16_1_from_u32(price));
		}
		break;

		case 3533: {//服务费率 0.0001元
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			price_info_t *price_info_service = &channels_info->channels_settings.price_info_service;
			uint8_t index = get_seg_index_by_ts(get_time());
			uint32_t price = price_info_service->price[index];
			modbus_data_value_r(modbus_data_ctx, get_u16_0_from_u32(price));
		}
		break;

		case 3534: {//开关机 (0:关机 1:开机)
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_rw(modbus_data_ctx, channel_info->display_cache_channel.onoff);
			channels_info->display_cache_channels.current_channel = 0;

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				if(channel_info->display_cache_channel.onoff == 0) {
					channel_info->display_cache_channel.charger_start_sync = 1;
				} else {
					if(channels_settings->authorize == 0) {
						channel_info->display_cache_channel.charger_start_sync = 1;
						channel_info->display_cache_channel.start_reason = CHANNEL_RECORD_ITEM_START_REASON_MANUAL;
						channel_info->display_cache_channel.charge_mode = CHANNEL_RECORD_CHARGE_MODE_UNLIMIT;
					}
				}
			}
		}
		break;

		case 3535: {//充电剩余时间 1分
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcs_data.remain_min);
		}
		break;

		case 3536: {//电池当前电量 1%
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcs_data.soc);
		}
		break;

		case 3537: {//电表电量读数 0.001度
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, get_u16_1_from_u32(channel_info->total_energy / 10));
		}
		break;

		case 3538: {//电表电量读数 0.001度
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, get_u16_0_from_u32(channel_info->total_energy / 10));
		}
		break;

		case 3539: {//显示屏下发消息
		}
		break;

		case 3540: {//主板上报提示
		}
		break;

		case 3541 ... 3549: {//当前充电VIN码
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_buffer_rw(modbus_data_ctx, (uint16_t *)channel_info->channel_record_item.vin, 17, modbus_data_ctx->addr - 3541);
		}
		break;

		case 3600: {//插枪状态
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			uint16_t value = 0;

			debug("charger %d connect state:%d", channel_info->channel_id, channel_info->charger_connect_state);
			if(channel_info->charger_connect_state == 1) {
				value = 1;
			}

			modbus_data_value_r(modbus_data_ctx, value);
		}
		break;

		case 3601: {//电子锁状态
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			channel_config_t *channel_config = channel_info->channel_config;

			modbus_data_value_r(modbus_data_ctx, channel_info->charger_lock_state);
		}
		break;

		case 3602: {//BMS状态
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->bms_state);
		}
		break;

		case 3603: {//BMS停机原因
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->channel_record_item.stop_reason);
		}
		break;

		case 3604: {//充电机停机原因
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->channel_record_item.stop_reason);
		}
		break;

		case 3605: {//充电机开机原因
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->channel_record_item.start_reason);
		}
		break;

		case 3606: {//绝缘电阻
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->insulation_resistor);
		}
		break;

		case 3607: {//绝缘状态
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->insulation_resistor);
		}
		break;

		case 3608: {//枪头正极温度
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->dc_p_temperature);
		}
		break;

		case 3609: {//枪头负极温度
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			modbus_data_value_r(modbus_data_ctx, channel_info->dc_n_temperature);
		}
		break;

		case 3610: {//电表连接状态
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			uint8_t fault = get_fault(channel_info->faults, CHANNEL_FAULT_ENERGYMETER);

			modbus_data_value_r(modbus_data_ctx, fault);
		}
		break;

		case 3611: {//查找Channel_Err_enum
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			int fault = get_first_fault(channels_info->faults);

			modbus_data_value_r(modbus_data_ctx, fault);
		}
		break;

		case 3612 ... 3613: {//channel_err_bit
		}
		break;

		case 3700: {//BMS版本号
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
			uint16_t version = charger_info->bms_data.chm_data.version_0;//todo

			modbus_data_value_r(modbus_data_ctx, version);
		}
		break;

		case 3701: {//电池类型 见地址表
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.brm_data.brm_data.battery_type);
		}
		break;

		case 3702: {//电池容量 0.1Ah
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.brm_data.brm_data.total_battery_rate_capicity);
		}
		break;

		case 3703: {//电池额定总电压 0.1V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.brm_data.brm_data.total_battery_rate_voltage);
		}
		break;

		case 3704: {//单体最高允许电压 0.01V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcp_data.max_charge_voltage);
		}
		break;

		case 3705: {//单体最高电压 0.01V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcs_data.u1.s.single_battery_max_voltage);
		}
		break;

		case 3706: {//单体最高允许温度 1° -50偏移量(-50 -- 200)
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcp_data.max_temperature);
		}
		break;

		case 3707: {//单体最高温度 1° -50偏移量(-50 -- 200)
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bsm_data.battery_max_temperature);
		}
		break;

		case 3708: {//BMS端测量充电电压 0.1V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcs_data.charge_voltage);
		}
		break;

		case 3709: {//BMS端测量充电电流 0.1A
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcs_data.charge_current);
		}
		break;

		case 3710: {//电池最大允许充电电压 0.1V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bhm_data.max_charge_voltage);
		}
		break;

		case 3711: {//充电前电池电压 0.1V
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcp_data.total_voltage);
		}
		break;

		case 3750: {//控制模式 (0:BMS模式  1:BMS手动  2:手动模式)
		}
		break;

		case 3751: {//手动模式下设定电压 0.1V
		}
		break;

		case 3752: {//手动模式下设定电流 0.1A
		}
		break;

		case 3753: {//辅助电源选择 (0:none 1:12V 2:24V) 查找AUX_Power_enum
			channel_info_t *channel_info = (channel_info_t *)channels_info->channel_info + channels_info->display_cache_channels.current_channel;
			charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;

			modbus_data_value_r(modbus_data_ctx, channel_info->auxiliary_power_type);
		}
		break;

		case 3754: {//条件充电模式 查找 Charge_Mode_Type 见地址表
		}
		break;

		case 3755: {//设定充电金额 0.01元
		}
		break;

		case 3756: {//BCD码 设定充电开始时间 小时 绝对时间
		}
		break;

		case 3757: {//BCD码 设定充电开始时间 分钟
		}
		break;

		case 3758: {//BCD码 设定充电结束时间 小时 绝对时间
		}
		break;

		case 3759: {//BCD码 设定充电结束时间 分钟
		}
		break;

		case 3760: {//设定充电电量 0.001kwh
		}
		break;

		case 3761: {//设定充电时间 1分钟 相对时间
		}
		break;

		case 3762: {//设定结束SOC
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
