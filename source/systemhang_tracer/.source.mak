# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================
ifeq ($(MTK_SYSTEM_HANG_TRACER_ENABLE),y)
CCFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE
ASFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE
endif

ifeq ($(MTK_SYSTEM_HANG_TRACER_ENABLE),o1)
CCFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE
CCFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE_O1
ASFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE
ASFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE_O1
endif

ifeq ($(MTK_SYSTEM_HANG_TRACER_ENABLE),o2)
CCFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE
CCFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE_O2
ASFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE
ASFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE_O2
endif

ifeq ($(MTK_SYSTEM_HANG_TRACER_ENABLE),mp)
CCFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE
CCFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE_MP
ASFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE
ASFLAG      +=  -DMTK_SYSTEM_HANG_TRACER_ENABLE_MP
endif


C_SRC		+=	\
$(addprefix source/systemhang_tracer/src/, \
systemhang_tracer.c     \
)

