

/*================================================================
 *
 *
 *   文件名称：test_storage_a.c
 *   创 建 者：肖飞
 *   创建日期：2022年07月12日 星期二 10时47分49秒
 *   修改日期：2022年07月12日 星期二 11时11分01秒
 *   描    述：
 *
 *================================================================*/
#include "test_storage_a.h"
#include "app.h"

typedef struct {
	callback_item_t periodic_callback_item;
	storage_info_t *storage_info;
	uint32_t stamps;
} test_storage_ctx_t;

static void storage_test_periodic(void *fn_ctx, void *chain_ctx)
{
	test_storage_ctx_t *test_storage_ctx = (test_storage_ctx_t *)fn_ctx;
	channels_info_t *channels_info = (channels_info_t *)chain_ctx;
	uint32_t ticks = osKernelSysTick();

	if(ticks_duration(ticks, test_storage_ctx->storage_info) >= 1000) {
		int ret = test_storage_check();
		uint8_t fault = 0;

		if(ret != 0) {
			fault = 1;
		}

		set_fault(channels_info->faults, CHANNELS_FAULT_STORAGE, fault);
	}
}

void start_storage_tests(channels_info_t *channels_info)
{
	test_storage_ctx_t *test_storage_ctx = os_calloc(1, sizeof(test_storage_ctx_t));
	app_info_t *app_info = get_app_info();

	OS_ASSERT(test_storage_ctx != NULL);
	OS_ASSERT(app_info != NULL);

	test_storage_ctx->storage_info = app_info->storage_info;
	test_storage_ctx->periodic_callback_item.fn = storage_test_periodic;
	test_storage_ctx->periodic_callback_item.fn_ctx = test_storage_ctx;
	OS_ASSERT(register_callback(channels_info->common_periodic_chain, &test_storage_ctx->periodic_callback_item) == 0);
}
