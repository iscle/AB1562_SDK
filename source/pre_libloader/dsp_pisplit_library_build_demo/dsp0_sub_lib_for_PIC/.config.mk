################################################################################
# Compiler and Commands Configuration
################################################################################
CROSS       := xt-
#           C Compiler
CC          :=  $(CROSS)xcc -c
#           Preprocessor
CCP         :=  $(CROSS)xcc -E
#           Assembler
AS          :=  $(CROSS)xcc -c
#           Archiver
AR          :=  $(CROSS)ar
#           Linker
LD          :=  $(CROSS)ld
OBJDUMP     :=  $(CROSS)objdump
OBJCOPY     :=  $(CROSS)objcopy
NM          :=  $(CROSS)nm
READELF     :=  readelf
RM          :=  dtidel
CP          :=  xcopy /T /Y /I

