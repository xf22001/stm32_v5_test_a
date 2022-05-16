

/*================================================================
 *   
 *   
 *   文件名称：test_ports_a.h
 *   创 建 者：肖飞
 *   创建日期：2022年05月16日 星期一 16时36分56秒
 *   修改日期：2022年05月16日 星期一 16时37分14秒
 *   描    述：
 *
 *================================================================*/
#ifndef _TEST_PORTS_A_H
#define _TEST_PORTS_A_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "app_platform.h"
#include "cmsis_os.h"

#include "channels.h"

void start_ports_tests(channels_info_t *channels_info);

#ifdef __cplusplus
}
#endif
#endif //_TEST_PORTS_A_H
