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
CONFIGDIR = $(shell xt-xcc --show-config=config)
include $(CONFIGDIR)/misc/hostenv.mk

TARGET       ?= DEBUG
################################################################################
# Project Configuration
################################################################################
ROOTDIR        :=  .
ROOTDIR_WIN    :=  .


SRCFOLDER      :=  src
SRCPATH        :=  $(ROOTDIR)
SRCPATH_WIN    :=  $(ROOTDIR_WIN)
SRCDIR         :=  $(SRCPATH)/$(SRCFOLDER)
SRCDIR_WIN     :=  $(SRCPATH_WIN)\$(SRCFOLDER)


OUTDIR         :=  $(ROOTDIR)/out
OUTDIR_WIN     :=  $(ROOTDIR)\out

LIBOUTDIR      :=  $(ROOTDIR)/lib
LIBOUTDIR_WIN  :=  $(ROOTDIR)\lib
LIB_NAME       :=  test.a
LIB            :=  $(LIBOUTDIR_WIN)\$(LIB_NAME)   

################################################################################
# Assmebly File
################################################################################
ASM_SRC        :=
 
 
################################################################################
# C File
################################################################################  
C_SRC          :=  
C_SRC          +=  src/sub_lib.c

################################################################################
# Include Path
################################################################################
INC            :=
INC            +=  inc

################################################################################
# Pattern List
################################################################################
OBJ          =  $(ASM_SRC:%.S=$(OUTDIR)/%.o) $(C_SRC:%.c=$(OUTDIR)/%.o)
DEP          =  $(ASM_SRC:%.S=$(OUTDIR)/%.d) $(C_SRC:%.c=$(OUTDIR)/%.d)

################################################################################
# Make and Include Folder Configuration
################################################################################

################################################################################
# Link and Compile Configuration
################################################################################
ASFLAG      :=
ASFLAG      +=  -W
ASFLAG      +=  -Wall
ASFLAG      +=  -g
ASFLAG      +=  -mlongcalls
ASFLAG      +=  -mtext-section-literals

CCFLAG      :=
CCFLAG      +=  -g
CCFLAG      +=  -W
CCFLAG      +=  -Wall
CCFLAG      +=  -Os
CCFLAG      +=  -O2
CCFLAG      +=  -std=gnu99
CCFLAG      +=  -INLINE:requested
CCFLAG      +=  -mlongcalls
CCFLAG      +=  -ffunction-sections
CCFLAG      +=  -mtext-section-literals
CFLAGS  +=  -O2 -Wall -g -fpic -mlongcalls -fmessage-length=0
ASFLAG  += -fpic
################################################################################
# Make Target
################################################################################
all : makesub $(OBJ) library

dbg :
	make all CCFLAG='-g -W -O0' LDFLAG='--gc-sections' DEFINE='BUILD_DEBUG BUILD_IODBG BUILD_CATCH BUILD_UARTDUMP BUILD_USR_ALLOC CONFIG_OS_FREERTOS'

clean_all :
	make clean

clean :
	@-rmdir $(OUTDIR_WIN) /Q /S
	@-rmdir $(LIBOUTDIR_WIN) /Q /S

makesub :    
ifeq "$(wildcard $(OUTDIR_WIN))" ""
	@mkdir $(OUTDIR_WIN)
endif
	$(CP) $(SRCDIR_WIN) $(OUTDIR_WIN)\$(SRCFOLDER)
ifeq "$(wildcard $(LIBOUTDIR_WIN))" ""
	@mkdir $(LIBOUTDIR_WIN)
endif

library : $(OBJ) ./makefile
	@$(AR) rcs -D  $(LIB) $(OBJ)

.PHONY: all clean dependents $(IMAGE) $(PIMAGE)

################################################################################
# Pattern Rule
################################################################################
$(OUTDIR)/%.o : $(SRCPATH)/%.S
	@echo Assembling... $<
	@$(AS) $(ASFLAG) $(INC:%=-I$(ROOTDIR)/"%") -MD -MF $(subst .o,.d,$@) -c -o $@ $<

$(OUTDIR)/%.o : $(SRCPATH)/%.c
	@echo Compiling... $<
	@$(CC) $(CCFLAG) $(INC:%=-I$(ROOTDIR)/"%") -MD -MF $(subst .o,.d,$@) -c -o $@ $<

################################################################################
# Include Dependency File
################################################################################
-include    $(DEP)

################################################################################
# Toolchain
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
SIZE        :=  $(CROSS)size
NM          :=  $(CROSS)nm
READELF     :=  readelf
#export CC CCP AS AR LD OBJDUMP OBJCOPY NM READELF
CP          :=  xcopy /T /Y /I


