

/*================================================================
 *   
 *   
 *   文件名称：test_storage_a.h
 *   创 建 者：肖飞
 *   创建日期：2022年07月12日 星期二 11时09分46秒
 *   修改日期：2022年07月12日 星期二 11时10分22秒
 *   描    述：
 *
 *================================================================*/
#ifndef _TEST_STORAGE_A_H
#define _TEST_STORAGE_A_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "app_platform.h"
#include "cmsis_os.h"

#include "channels.h"

void start_storage_tests(channels_info_t *channels_info);

#ifdef __cplusplus
}
#endif
#endif //_TEST_STORAGE_A_H
