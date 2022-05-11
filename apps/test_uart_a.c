

/*================================================================
 *
 *
 *   文件名称：test_uart_a.c
 *   创 建 者：肖飞
 *   创建日期：2022年05月09日 星期一 09时04分44秒
 *   修改日期：2022年05月11日 星期三 08时55分15秒
 *   描    述：
 *
 *================================================================*/
#include "test_uart_a.h"

#include "uart_data_task.h"

#include "log.h"

static void uart_periodic(void *fn_ctx, void *chain_ctx)
{
	uart_test_ctx_t *ctx = (uart_test_ctx_t *)fn_ctx;
	channels_info_t *channels_info = (channels_info_t *)chain_ctx;
	uint32_t ticks = osKernelSysTick();
	uint8_t fault = 0;
	int uart_fault = -1;

	switch(ctx->id) {
		case 0: {
			uart_fault = CHANNELS_FAULT_UART1;
		}
		break;

		case 1: {
			uart_fault = CHANNELS_FAULT_UART2;
		}
		break;

		case 2: {
			uart_fault = CHANNELS_FAULT_UART3;
		}
		break;

		case 3: {
			uart_fault = CHANNELS_FAULT_UART4;
		}
		break;

		case 4: {
			uart_fault = CHANNELS_FAULT_UART5;
		}
		break;

		default: {
			app_panic();
		}
		break;
	}

	if(ticks_duration(ticks, ctx->alive_stamps) >= 1000) {
		fault = 1;
	}

	if(get_fault(channels_info->faults, uart_fault) != fault) {
		set_fault(channels_info->faults, uart_fault, fault);
	}
}

typedef struct {
	int id;
	uint32_t v1;
	uint32_t v2;
} uart_test_tx_data_t;

typedef struct {
	int id;
	uint32_t v;
} uart_test_rx_data_t;

static void uart_data_request(void *fn_ctx, void *chain_ctx)
{
	uart_test_ctx_t *ctx = (uart_test_ctx_t *)fn_ctx;
	uart_data_task_info_t *uart_data_task_info = (uart_data_task_info_t *)chain_ctx;
	uart_test_tx_data_t *uart_test_tx_data = (uart_test_tx_data_t *)ctx->tx_data;
	uint16_t rx_size;
	uint32_t ticks = osKernelSysTick();

	uart_test_tx_data->id = ctx->id;
	uart_test_tx_data->v1 = os_rand();
	uart_test_tx_data->v2 = os_rand();
	ctx->tx_size = sizeof(uart_test_tx_data_t);

	ctx->rx_size = sizeof(uart_test_rx_data_t);

	rx_size = uart_tx_rx_data(uart_data_task_info->uart_info, ctx->tx_data, ctx->tx_size, ctx->rx_data, ctx->rx_size, 100);

	if(rx_size == ctx->rx_size) {
		uart_test_rx_data_t *uart_test_rx_data = (uart_test_rx_data_t *)ctx->rx_data;

		if(uart_test_rx_data->id == ctx->id) {
			uint32_t v = uart_test_tx_data->v1 + uart_test_tx_data->v2;

			if(uart_test_rx_data->v == v) {
				ctx->alive_stamps = ticks;
			} else {
				debug("uart %d uart_test_rx_data->v:%u, v:%u", ctx->id, uart_test_rx_data->v, v);
			}
		} else {
			debug("uart %d uart_test_rx_data->id:%d, ctx->id:%d", ctx->id, uart_test_rx_data->id, ctx->id);
		}
	} else {
		debug("uart %d rx_size:%d, ctx->rx_size:%d", ctx->id, rx_size, ctx->rx_size);
	}
}

uart_test_ctx_t *test_uart_a(channels_info_t *channels_info, void *huart, uint8_t id)
{
	uart_test_ctx_t *ctx = os_calloc(1, sizeof(uart_test_ctx_t));
	uart_data_task_info_t *uart_data_task_info = get_or_alloc_uart_data_task_info(huart);

	OS_ASSERT(ctx != 0);
	OS_ASSERT(uart_data_task_info != 0);

	set_uart_data_task_request_delay(uart_data_task_info, 200);
	uart_info_signal(uart_data_task_info->uart_info);

	ctx->id = id;

	ctx->data_request_cb.fn = uart_data_request;
	ctx->data_request_cb.fn_ctx = ctx;
	OS_ASSERT(add_uart_data_task_info_cb(uart_data_task_info, &ctx->data_request_cb) == 0);

	ctx->periodic_callback_item.fn = uart_periodic;
	ctx->periodic_callback_item.fn_ctx = ctx;
	OS_ASSERT(register_callback(channels_info->common_periodic_chain, &ctx->periodic_callback_item) == 0);

	return ctx;
}
