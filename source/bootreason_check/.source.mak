# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================
ifeq ($(MTK_BOOTREASON_CHECK_ENABLE),y)
CCFLAG      +=  -DMTK_BOOTREASON_CHECK_ENABLE
endif


C_SRC		+=	\
$(addprefix source/bootreason_check/src/, \
bootreason_check.c     \
)

C_SRC		+=	\
$(addprefix source/bootreason_check/portable/ab1562/, \
bootreason_portable.c     \
)

