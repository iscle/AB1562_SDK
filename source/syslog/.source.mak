# ==============================================================================
# add all .S and .c files in sub-folder of source/syslog/ into target source
# ==============================================================================
ifeq ($(MTK_MUX_ENABLE), y)
C_SRC		+=	$(addprefix source/syslog/src/,syslog_version_2_variant.c)
C_SRC		+=	$(addprefix source/syslog/src/,syslog_dump_buffer_flash.c)
C_SRC		+=	$(addprefix source/syslog/src/,syslog_debug.c)
endif

