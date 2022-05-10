# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================


C_SRC		+=	\
$(addprefix source/offline_dump/src/, \
offline_dump.c     \
)

