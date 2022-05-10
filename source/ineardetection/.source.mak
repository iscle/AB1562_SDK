# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================

C_SRC		+=	\
$(addprefix source/ineardetection/src/, \
in_ear_detection.c \
drv_sensor.c \
)
