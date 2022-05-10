# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================

C_SRC		+=	\
$(addprefix source/hal/src/, \
hal_clock.c \
hal_dcxo.c \
hal_dvfs_internal_vcore.c \
hal_dvfs.c \
hal_i2c_master.c \
hal_trng.c	\
hal_trng_internal.c	\
hal_uart.c \
hal_uart_internal.c \
hal_spi_master.c \
hal_spi_master_internal.c \
hal_adc.c \
hal_gpio.c \
hal_gpio_internal.c \
hal_eint.c \
hal_eint_internal.c \
hal_captouch.c \
hal_pwm.c \
hal_nvic.c \
hal_nvic_internal.c \
hal_gpt.c \
hal_gpt_internal.c \
hal_pdma_internal.c \
hal_sw_dma_internal.c \
hal_sw_dma.c \
hal_cache_internal.c \
hal_cache.c \
hal_flash_combo_init.c \
hal_flash_combo_nor.c \
hal_flash_custom.c \
hal_flash_custom_sfi.c \
hal_flash_disk.c \
hal_flash_mtd.c \
hal_flash_mtd_common.c \
hal_flash_mtd_dal.c \
hal_wdt.c \
hal_wdt_internal.c \
hal_dwt.c \
hal_rtc_internal.c \
hal_rtc.c \
hal_spm.c \
hal_sleep_manager.c \
hal_sleep_manager_internal.c \
hal_core_status.c \
hal_time_check.c \
hal_gpio_config.c \
smartcharger_1wire.c \
)

ASM_SRC                +=      \
$(addprefix source/hal/src/, \
sleep_exit_prepare.S \
)
