# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================
C_SRC		+=	\
$(addprefix source/user_data/src/, \
user_data.c     \
user_data_example.c     \
)

