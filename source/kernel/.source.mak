# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================
C_SRC		+=	\
$(addprefix source/kernel/service/, \
log.c \
os_memory.c \
os_list.c   \
swla.c     \
)

ASM_SRC		+=	\
$(addprefix source/kernel/service/, \
os_memory_asm.S \
)

C_SRC		+=	\
$(addprefix source/kernel/FreeRTOS/, \
event_groups.c \
list.c \
queue.c \
tasks.c \
timers.c \
)

C_SRC		+=	\
$(addprefix source/kernel/FreeRTOS/portable/MemMang/, \
heap_4.c \
)

C_SRC		+=	\
$(addprefix source/kernel/FreeRTOS/portable/MemMang/multi_pool/src/, \
multi_pool_heap.c \
)

C_SRC		+=	\
$(addprefix source/kernel/FreeRTOS/portable/Trace/, \
os_trace_callback.c \
)

C_SRC		+=	\
$(addprefix source/kernel/FreeRTOS/portable/XCC/Xtensa/, \
port.c \
port_tick.c \
portclib.c \
xtensa_init.c \
xtensa_intr.c \
xtensa_overlay_os_hook.c \
)

ASM_SRC		+=	\
$(addprefix source/kernel/FreeRTOS/portable/XCC/Xtensa/, \
xtensa_vectors.S \
xtensa_intr_asm.S \
xtensa_context.S \
portasm.S \
)
