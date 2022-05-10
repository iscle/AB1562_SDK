# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================
C_SRC		+=	\
$(addprefix source/FS/, \
diskio.c \
ff.c \
)

C_SRC		+=	\
$(addprefix source/FS/option/, \
syscall.c \
unicode.c \
)
