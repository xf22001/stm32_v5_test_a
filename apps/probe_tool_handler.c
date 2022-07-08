

/*================================================================
 *
 *
 *   文件名称：probe_tool_handler.c
 *   创 建 者：肖飞
 *   创建日期：2020年03月20日 星期五 12时48分07秒
 *   修改日期：2022年07月08日 星期五 09时13分18秒
 *   描    述：
 *
 *================================================================*/
#include "probe_tool_handler.h"

#include <string.h>

#include "lwip/netdb.h"
#include "lwip/inet.h"

#include "app.h"
#include "channels.h"
#include "iap.h"

#include "sal_hook.h"

#define LOG_UDP
#include "log.h"

static void fn1(request_t *request)
{
	probe_server_chunk_sendto(request->payload.fn, (void *)0x8000000, 512);
}

#include "test_event.h"
static void fn2(request_t *request)
{
	try_get_test_event();
}

static int p_host(struct hostent *ent)
{
	int ret = 0;
	char **cp;

	if(ent == NULL) {
		ret = -1;
		return ret;
	}

	_printf("\n");

	_printf("h_name:%s\n", ent->h_name);
	_printf("h_aliases:\n");
	cp = ent->h_aliases;

	while(*cp != NULL) {
		_printf("%s\n", *cp);
		cp += 1;

		if(*cp != NULL) {
			//_printf(", ");
		}
	}

	_printf("h_addrtype:%d\n", ent->h_addrtype);

	_printf("h_length:%d\n", ent->h_length);

	_printf("h_addr_list:\n");
	cp = ent->h_addr_list;

	while(*cp != NULL) {
		_printf("%s\n", inet_ntoa(**cp));
		cp += 1;

		if(*cp != NULL) {
			//_printf(", ");
		}
	}

	return ret;
}

static void get_host_by_name(char *content, uint32_t size)
{
	struct hostent *ent;
	char *hostname = (char *)os_alloc(RECV_BUFFER_SIZE);
	int ret;
	int fn;
	int catched;

	//_hexdump("content", (const char *)content, size);

	if(hostname == NULL) {
		return;
	}

	hostname[0] = 0;

	ret = sscanf(content, "%d %s%n", &fn, hostname, &catched);

	if(ret == 2) {
		_printf("hostname:%s!\n", hostname);
		ent = gethostbyname(hostname);
		p_host(ent);
	} else {
		_printf("no hostname!\n");
	}

	os_free(hostname);
}

static void fn4(request_t *request)
{
	const ip_addr_t *local_ip = get_default_ipaddr();
	_printf("local host ip:%s\n", inet_ntoa(*local_ip));

	get_host_by_name((char *)(request + 1), request->header.data_size);
	memset(request, 0, RECV_BUFFER_SIZE);
}

uint16_t osGetCPUUsage(void);
int get_brk_size(void);
static void fn5(request_t *request)
{
	int size = xPortGetFreeHeapSize();
	uint8_t *os_thread_info;
	uint32_t ticks = osKernelSysTick();
	uint16_t cpu_usage = osGetCPUUsage();
	size_t total_heap_size = get_total_heap_size();
	size_t heap_size;
	size_t heap_count;
	size_t heap_max_size;

	get_mem_info(&heap_size, &heap_count,  &heap_max_size);

	_printf("cpu usage:%d\n", cpu_usage);
	_printf("free os heap size:%d\n", size);
	_printf("brk size:%d\n", get_brk_size());
	_printf("total heap size:%d, free heap size:%d, used:%d, heap count:%d, max heap size:%d\n",
	        total_heap_size,
	        total_heap_size - heap_size,
	        heap_size,
	        heap_count,
	        heap_max_size);
	_printf("current ticks:%lu\n", ticks);
	_printf("%lu day %lu hour %lu min %lu sec\n",
	        ticks / (1000 * 60 * 60 * 24),//day
	        (ticks % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60),//hour
	        (ticks % (1000 * 60 * 60)) / (1000 * 60),//min
	        (ticks % (1000 * 60)) / (1000)//sec
	       );

	if(size < 4 * 1024) {
		return;
	}

	size = 1024;

	os_thread_info = (uint8_t *)os_calloc(1, size);

	if(os_thread_info == NULL) {
		return;
	}

	osThreadList(os_thread_info);

	_printf("%-15s\t%s\t%s\t%s\t%s\n", "name", "state", "prio", "stack", "no");
	_puts((const char *)os_thread_info);

	vTaskGetRunTimeStats((char *)os_thread_info);

	_printf("\n\n%-15s\t%s\t\t%s\n", "name", "count", "percent");
	_puts((const char *)os_thread_info);

	_printf("\n");

	os_free(os_thread_info);

	if(is_app() == 1) {
		_printf("in app!\n");
	} else {
		_printf("in bootloader!\n");
	}
}


static void fn8(request_t *request)
{
}

static void fn9(request_t *request)
{
}

static void fn13(request_t *request)
{
	char *content = (char *)(request + 1);
	int fn;
	int catched;
	int ret;
	struct tm tm;
	time_t ts;

	ret = sscanf(content, "%d %04d%02d%02d%02d%02d%02d %n",
	             &fn,
	             &tm.tm_year,
	             &tm.tm_mon,
	             &tm.tm_mday,
	             &tm.tm_hour,
	             &tm.tm_min,
	             &tm.tm_sec,
	             &catched);
	debug("ret:%d", ret);

	if(ret == 7) {
		tm.tm_year -= 1900;
		tm.tm_mon -= 1;
		ts = mktime(&tm);

		if(set_time(ts) == 0) {
			debug("set time successful!");
		} else {
			debug("set time failed!");
		}
	}

	ts = get_time();
	tm = *localtime(&ts);
	debug("tm %04d-%02d-%02d %02d:%02d:%02d",
	      tm.tm_year + 1900,
	      tm.tm_mon + 1,
	      tm.tm_mday,
	      tm.tm_hour,
	      tm.tm_min,
	      tm.tm_sec);
}

static server_item_t server_map[] = {
	{1, fn1},
	{2, fn2},
	{4, fn4},
	{5, fn5},
	{8, fn8},
	{9, fn9},
	{13, fn13},
};

server_map_info_t server_map_info = {
	.server_map = server_map,
	.server_map_size = sizeof(server_map) / sizeof(server_item_t),
};
