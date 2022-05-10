# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================

C_SRC		+=	\
$(addprefix source/sha_info/, \
sha_number.c \
)
