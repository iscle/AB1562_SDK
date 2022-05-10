# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================

C_SRC		+=	\
$(addprefix source/lzma1701/C/, \
LzmaDec.c \
Aes.c \
)
