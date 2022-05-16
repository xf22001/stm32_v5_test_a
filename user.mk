#
#
#================================================================
#   
#   
#   文件名称：user.mk
#   创 建 者：肖飞
#   创建日期：2019年10月25日 星期五 13时04分38秒
#   修改日期：2022年05月16日 星期一 16时37分43秒
#   描    述：
#
#================================================================

include config.mk

ifndef_any_of = $(filter undefined,$(foreach v,$(1),$(origin $(addprefix CONFIG_,$(v)))))
ifdef_any_of = $(filter-out undefined,$(foreach v,$(1),$(origin $(addprefix CONFIG_,$(v)))))

USER_C_INCLUDES += -Iapps
USER_C_INCLUDES += -Iapps/modules
USER_C_INCLUDES += -Iapps/modules/os
USER_C_INCLUDES += -Iapps/modules/drivers
ifneq ($(call ifdef_any_of,SPI_CAN),)
USER_C_INCLUDES += -Iapps/modules/drivers/mcp2518
USER_C_INCLUDES += -Iapps/modules/drivers/mcp2518/mcp25xxfd_driver/canfdspi
USER_C_INCLUDES += -Iapps/modules/drivers/mcp2518/mcp25xxfd_driver/spi
endif
USER_C_INCLUDES += -Iapps/modules/hardware
USER_C_INCLUDES += -Iapps/modules/app
USER_C_INCLUDES += -Iapps/modules/tests

USER_C_SOURCES += apps/os_memory.c
USER_C_SOURCES += apps/os_random.c
USER_C_SOURCES += apps/local_time.c
USER_C_SOURCES += apps/app.c
USER_C_SOURCES += apps/uart_debug_handler.c
USER_C_SOURCES += apps/can_config.c
USER_C_SOURCES += apps/channels_config.c
USER_C_SOURCES += apps/channels.c
USER_C_SOURCES += apps/test_can_a.c
USER_C_SOURCES += apps/test_uart_a.c
USER_C_SOURCES += apps/test_ports_a.c
ifneq ($(call ifdef_any_of,SPI_CAN),)
USER_C_SOURCES += apps/exti.c
endif

USER_C_SOURCES += apps/modules/app/uart_debug.c
USER_C_SOURCES += apps/modules/app/can_data_task.c
USER_C_SOURCES += apps/modules/app/uart_data_task.c
#USER_C_SOURCES += apps/modules/app/duty_cycle_pattern.c
USER_C_SOURCES += apps/modules/app/early_sys_callback.c
USER_C_SOURCES += apps/modules/app/connect_state.c
USER_C_SOURCES += apps/modules/app/can_command.c
USER_C_SOURCES += apps/modules/app/display.c
USER_C_SOURCES += apps/modules/hardware/modbus_master_txrx.c
USER_C_SOURCES += apps/modules/hardware/modbus_slave_txrx.c
USER_C_SOURCES += apps/modules/hardware/modbus_spec.c
USER_C_SOURCES += apps/modules/drivers/spi_txrx.c
USER_C_SOURCES += apps/modules/drivers/can_txrx.c
USER_C_SOURCES += apps/modules/drivers/can_ops_hal.c
ifneq ($(call ifdef_any_of,SPI_CAN),)
USER_C_SOURCES += apps/modules/drivers/mcp2518/mcp25xxfd_driver/canfdspi/drv_canfdspi_api.c
USER_C_SOURCES += apps/modules/drivers/can_ops_spi_can.c
endif
USER_C_SOURCES += apps/modules/drivers/usart_txrx.c
USER_C_SOURCES += apps/modules/os/event_helper.c
USER_C_SOURCES += apps/modules/os/callback_chain.c
USER_C_SOURCES += apps/modules/os/bitmap_ops.c
USER_C_SOURCES += apps/modules/os/os_utils.c
USER_C_SOURCES += apps/modules/os/cpu_utils.c
USER_C_SOURCES += apps/modules/os/log.c
USER_C_SOURCES += apps/modules/os/object_class.c
USER_C_SOURCES += apps/modules/os/soft_timer.c
USER_C_SOURCES += apps/modules/os/retarget.c
USER_C_SOURCES += apps/modules/os/syscalls.c
USER_C_SOURCES += apps/modules/tests/test_serial.c
USER_C_SOURCES += apps/modules/tests/test_event.c
USER_C_SOURCES += apps/modules/tests/test_storage.c
USER_C_SOURCES += apps/modules/tests/test_can.c

USER_CFLAGS += -DtraceTASK_SWITCHED_IN=StartIdleMonitor -DtraceTASK_SWITCHED_OUT=EndIdleMonitor
USER_CFLAGS += -DSAL_HOOK
USER_CFLAGS += -DLOG_CONFIG_FILE=\"log_config.h\"
USER_CFLAGS += -DCJSON_API_VISIBILITY -DCJSON_EXPORT_SYMBOLS -DENABLE_LOCALES -Dcjson_EXPORTS

#USER_CFLAGS += -DLOG_DISABLE
#USER_CFLAGS += -DALLOC_TRACE_DISABLE

CFLAGS += $(USER_CFLAGS) $(CONFIG_CFLAGS)

#LDFLAGS += -u _printf_float -Wl,--wrap=srand  -Wl,--wrap=rand
LDFLAGS += -u _printf_float

default: all

LDSCRIPT = STM32F407IGTx_FLASH.ld

PHONY += all
PHONY += default

USER_DEPS := config.mk $(build-type) $(LDSCRIPT)

cscope: all
	rm cscope e_cs -rf
	mkdir -p cscope
	#$(silent)tags.sh prepare;
	$(silent)touch dep_files;
	$(silent)touch raw_dep_files;
	$(silent)for f in $$(find . -type f -name "*.d" 2>/dev/null); do \
		cat "$$f" >> raw_dep_files; \
	done;
	for i in $$(cat "raw_dep_files" | sed 's/^.*://g' | sed 's/[\\ ]/\n/g' | sort -h | uniq); do \
		if test "$${i:0:1}" = "/";then \
			echo "$$i" >> dep_files; \
		else \
			readlink -f "$$i" >> dep_files; \
		fi; \
	done; \
	$(silent)rm raw_dep_files
	$(silent)cat dep_files | sort | uniq | sed 's/^\(.*\)$$/\"\1\"/g' >> cscope/cscope.files;
	$(silent)cat dep_files | sort | uniq >> cscope/ctags.files;
	$(silent)rm dep_files
	$(silent)tags.sh cscope;
	$(silent)tags.sh tags;
	$(silent)tags.sh env;

clean: clean-cscope
clean-cscope:
	rm cscope e_cs -rf

firmware:
	python apps/modules/fw.py -f build/eva.bin

