# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================
C_SRC		+=	\
$(addprefix source/exception_handler/src/, \
exception_handler.c     \
)

C_SRC		+=	\
$(addprefix source/exception_handler/portable/ab1562/, \
exception_portable.c     \
)

ASM_SRC		+=	\
$(addprefix source/exception_handler/portable/xtensa/, \
exception.S \
)
