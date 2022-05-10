# ==============================================================================
# add all .S and .c files in sub-folder of source/mux/ into target source
# ==============================================================================


ifeq ($(MTK_MUX_ENABLE),y)
CCFLAG += -DMTK_MUX_ENABLE
endif

C_SRC		+=	\
$(addprefix source/mux/src/, \
mux.c \
mux_port.c \
mux_port_device.c \
mux_uart.c \
mux_airapp.c \
)

# ==============================================================================
# no need 
# ==============================================================================
# C_SRC		+=	\
# $(addprefix source/mux/src/, \
# mux_i2c_slave.c \
# mux_spi_slave.c \
# mux_flash.c \
# mux_usb.c \
# )

