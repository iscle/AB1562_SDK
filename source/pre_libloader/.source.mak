# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================
CCFLAG      +=  -DPRELOADER_ENABLE
C_SRC		+=	\
$(addprefix source/pre_libloader/src/, \
common.c \
pi_library_load.c \
pi_library_symbols.c \
pi_relocate_lib.c \
preloader_pisplit.c \
preloader_pisplit_internal.c \
preloader_pisplit_configure.c \
)

#CCFLAG      +=  -DDSP0_PISPLIT_DEMO_LIBRARY
PIC     += $(ROOTDIR)/source/pre_libloader/dsp_pisplit_library_build_demo/dsp0/Build/lib/dsp0_pisplit_library_build_demo.o
C_SRC		+=	\
$(addprefix source/pre_libloader/dsp_pic_demo_portable/, \
dsp0_pisplit_demo_library_portable.c \
)