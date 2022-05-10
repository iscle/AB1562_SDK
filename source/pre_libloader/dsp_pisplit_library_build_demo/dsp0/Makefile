# Copyright Statement:
#
# (C) 2017  Airoha Technology Corp. All rights reserved.
#
# This software/firmware and related documentation ("Airoha Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to Airoha Technology Corp. ("Airoha") and/or its licensors.
# Without the prior written permission of Airoha and/or its licensors,
# any reproduction, modification, use or disclosure of Airoha Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
# You may only use, reproduce, modify, or distribute (as applicable) Airoha Software
# if you have agreed to and been bound by the applicable license agreement with
# Airoha ("License Agreement") and been granted explicit permission to do so within
# the License Agreement ("Permitted User").  If you are not a Permitted User,
# please cease any access or use of Airoha Software immediately.
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT AIROHA SOFTWARE RECEIVED FROM AIROHA AND/OR ITS REPRESENTATIVES
# ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. AIROHA EXPRESSLY DISCLAIMS ANY AND ALL
# WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES AIROHA PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH AIROHA SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN AIROHA SOFTWARE. AIROHA SHALL ALSO NOT BE RESPONSIBLE FOR ANY AIROHA
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AIROHA'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO AIROHA SOFTWARE RELEASED HEREUNDER WILL BE,
# AT AIROHA'S OPTION, TO REVISE OR REPLACE AIROHA SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# AIROHA FOR SUCH AIROHA SOFTWARE AT ISSUE.
#

#------------------------------------------------------------------------------
# See README.TXT for more information.
#------------------------------------------------------------------------------

#CONFIGDIR   = $(shell xt-xcc --show-config=config)
# For platform-specific commands
#include $(CONFIGDIR)/misc/hostenv.mk

TARGET      ?=  DEBUG
PROJECT     ?=  AB1562
PLATFORM    ?=  ASIC
RF          ?=  RF1530C
MODE        ?=  DEBUG
OS           =  FREERTOS
LDFLAG      :=
CCFLAG      :=

ROOTDIR       :=  ../../../..
ROOTDIR_WIN    :=  ..\..\..\..
OUTDIR        ?=  Build
BUILD_LIBDIR  :=  $(strip $(OUTDIR))/lib
LOGDIR        :=  $(strip $(OUTDIR))/log
ERR_LOG       :=  $(strip $(LOGDIR))/err.log
BUILD_LOG     :=  $(strip $(LOGDIR))/build.log

# PIC settings
PIC      :=  dsp0_pisplit_library_build_demo
# PIC_LIB named better is {PIC}.o
PIC_LIB  :=  $(BUILD_LIBDIR)/dsp0_pisplit_library_build_demo.o
# PIC_DEBUG_LIB named better is {PIC}__position_independent.splitload.lib
PIC_DEBUG_LIB  :=  $(BUILD_LIBDIR)/dsp_pisplit_library_build_demo_position_independent.splitload.lib
PIC_MODULE_PATH := /source/pre_libloader/dsp_pisplit_library_build_demo/dsp0


# info from project makefile
#INC     += $(PROJ_INC)
#CCFLAG  += $(PROJ_CCFLAG)
#ASFLAG  += $(PROJ_ASFLAG)

# Compile option settings

ASFLAG      :=
#ASFLAG      +=  -dbg
ASFLAG      +=  -W
ASFLAG      +=  -Wall
#ASFLAG      +=  -Wundef
#ASFLAG      +=  -L
ASFLAG      +=  -g
#ASFLAG      +=  -D
#ASFLAG      +=  -g --gen-debug
#ASFLAG      +=  --gdwarf-2
ASFLAG      +=  -mlongcalls
ASFLAG      +=  -DFREERTOS=1
#ASFLAG      +=  -mtext-section-literals
#ASFLAG      +=  --text-section-literals


CCFLAG      +=  -g
CCFLAG      +=  -W
CCFLAG      +=  -Wall
CCFLAG      +=  -Os
CCFLAG      +=  -O2
CCFLAG      +=  -std=gnu99
CCFLAG      +=  -INLINE:requested
#CCFLAG      +=  -mno-fp-as-gp
CCFLAG      +=  -mlongcalls
#CCFLAG      +=  -fdata-sections
CCFLAG      +=  -ffunction-sections
CCFLAG      +=  -mtext-section-literals
CCFLAG      +=  -DFREERTOS=1
CCFLAG      +=  -DFREERTOS_ENABLE
CCFLAG      +=  -DMTK_SWLA_ENABLE
CCFLAG      +=  -DMTK_SUPPORT_HEAP_DEBUG
#CCFLAG      +=  -DMTK_HEAP_SIZE_GUARD_ENABLE
CCFLAG      +=  -DCORE_DSP0

# mbedtls
CCFLAG      +=  -DMBEDTLS_CONFIG_FILE="\"$(APPSRCPATH)/inc/mbedtls-config-AB1562.h\""

# lzma
CCFLAG      +=  -D_7ZIP_ST

LDFLAG      +=  --gc-sections
#LDFLAG      +=  --print-gc-sections
#LDFLAG      +=  --relax
#LDFLAG      +=  --xt-map

# Source file settings
C_SRC  +=  $(strip $(PIC_MODULE_PATH))/src/lib_main.c
ASM_SRC      +=

# Include path settings
INC  +=  $(strip $(PIC_MODULE_PATH))/inc

# .o objects & .d dependency list
PICSL_LIBRARY_OBJS = $(C_SRC:%.c=$(OUTDIR)/%.picsl.o)
#$(info $(PICSL_LIBRARY_OBJS))
PICSL_LIBRARY_OBJS += $(ASM_SRC:%.S=$(OUTDIR)/%.picsl.o)
PICSL_LIBRARY_OBJS += $(ROOTDIR)/source/pre_libloader/dsp_pisplit_library_build_demo/dsp0/dsp0_sub_lib_demo.a
#DEP  =  $(ASM_SRC:%.S=$(OUTDIR)/%.d) $(C_SRC:%.c=$(OUTDIR)/%.d)

# Targets
.PHONY : all clean

all : $(PIC_LIB)
	@echo Build $(PIC_LIB) Done


$(OUTDIR)/%.picsl.o :  $(ROOTDIR)/%.S
	@mkdir $(subst /,\,$(dir $@))
	xt-xcc -c  $(ASFLAG) $(INC:%=-I"$(ROOTDIR)/%") -fpic -o $@ -c $^

$(OUTDIR)/%.picsl.o :  $(ROOTDIR)/%.c
	@mkdir $(subst /,\,$(dir $@))
	xt-xcc -c $(CFLAGS) $(INC:%=-I"$(ROOTDIR)/%") -fpic -o $@ -c $^
	
$(PIC_DEBUG_LIB) : $(PICSL_LIBRARY_OBJS)
#	@if [ -e "$@" ]; then rm -f "$@"; fi
	@mkdir $(subst /,\,$(dir $@))
	xt-xcc $(CFLAGS) -mlsp=pisplitload -Wl,--shared-pagesize=128  $^ \
	-Wl,-pie -o $@
	xt-objdump -d $@ > $@.asm
	
$(PIC_LIB) : $(PIC_DEBUG_LIB)
#	@if [ -e "$@" ]; then rm -f "$@"; fixt
ifeq "$(wildcard Build\lib)" ""
	@mkdir $(subst /,\,$(dir $@))
endif
	xt-pkg-loadlib -p -e $(PIC) -o $@ $^

clean :
	rmdir Build /Q/S
	
#include $(ROOTDIR)/.rule.mk
-include $(DEP)
