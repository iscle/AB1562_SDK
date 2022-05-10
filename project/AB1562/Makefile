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

TARGET      ?=  DEBUG
PROJECT     ?=  AB1562
PLATFORM    ?=  ASIC
RF          ?=  RF1530C
MODE        ?=  DEBUG
OS           =  FREERTOS
FEATURE     ?=  .feature.mak
FILE_XML    ?=  FileManagement.xml
LDFLAG      :=
CCFLAG      :=
<<<<<<< HEAD

=======
ASFLAG      :=
>>>>>>> db20e11 (second commit)

RAM_BOOT    ?=  n


################################################################################
# Project Configuration
################################################################################
ROOTDIR        :=  ../..
ROOTDIR_WIN    :=  ..\..

SRCFOLDER      :=  source
SRCPATH        :=  $(ROOTDIR)
SRCPATH_WIN    :=  $(ROOTDIR_WIN)
SRCDIR         :=  $(SRCPATH)/$(SRCFOLDER)
SRCDIR_WIN     :=  $(SRCPATH_WIN)\$(SRCFOLDER)

APPSRCFOLDER   :=  src
APPROOTDIR_WIN :=  $(subst /,\,$(APPROOTDIR))
APPSRCPATH     :=  $(ROOTDIR)/$(APPROOTDIR)
APPSRCPATH_WIN :=  $(ROOTDIR_WIN)\$(APPROOTDIR_WIN)
APPSRCDIR      :=  $(APPSRCPATH)/$(APPSRCFOLDER)
APPSRCDIR_WIN  :=  $(APPSRCPATH_WIN)\$(APPSRCFOLDER)

PUBLICINC      :=  include
PUBPATH        :=  $(ROOTDIR)
PUBPATH_WIN    :=  $(ROOTDIR_WIN)
PUBDIR         :=  $(PUBPATH)/$(PUBLICINC)
PUBDIR_WIN     :=  $(PUBPATH)\$(PUBLICINC)

VERSION        ?=
OUTDIR         :=  $(ROOTDIR)/out/$(PROJECT)/$(APP)
OUTDIR_WIN     :=  $(ROOTDIR_WIN)\out\$(PROJECT)\$(APP)
OUTPATH_WIN    :=  $(ROOTDIR_WIN)\tools\debug_Environment\AB1562\debug
IMAGE          :=  $(OUTDIR)/AB1562$(VERSION).out
BINARY         :=  $(OUTDIR)/AB1562$(VERSION).bin
HEX            :=  $(OUTDIR)/AB1562$(VERSION).hex
DEASM          :=  $(OUTDIR)/AB1562$(VERSION).asm
MAP            :=  $(OUTDIR)/AB1562$(VERSION).map
LOG            :=  $(OUTDIR)/AB1562_log$(VERSION).bin
IRAM_COMP_BIN  :=  $(OUTDIR)/AB1562$(VERSION)_compress.bin
IRAM_COMP_BIN_W:=  $(OUTDIR_WIN)\AB1562$(VERSION)_compress.bin
IRAM_BIN       :=  $(OUTDIR)/AB1562$(VERSION)_IRAM.bin
IRAM_COMP_LZMA :=  $(OUTDIR)/AB1562$(VERSION)_IRAM.bin.lzma

NVKEY_XML      ?=  nvkey.xml
NVKEY_FOTA_XML ?=  nvkey_fota.xml
SYSKEY_FOTA_XML?=  AB1530B_syskey_fota.xml
SYSKEY_XML     ?=  AB1530B_syskey.xml

BOOTLOADER      = $(OUTDIR)\AB1562_bootloader.o

FILES           = $(OUTDIR)\filesystem.o
NVKEY_NOFOTA    = $(OUTDIR)\nvkey_nofota_bin.o
NVKEY_FOTA      = $(OUTDIR)\nvkey_fota_bin.o

IRAM_COMPRESS   = $(OUTDIR)/iram_compress.o

LIBMIDDIR      :=  $(ROOTDIR)/prebuilt/middleware
LIBMIDDIR_WIN  :=  $(ROOTDIR_WIN)\prebuilt\middleware
LIBSYSDIR      :=  $(ROOTDIR)/prebuilt/system
LIBSYSDIR_WIN  :=  $(ROOTDIR_WIN)\prebuilt\system
LIBPRIDIR      :=  $(ROOTDIR)/prebuilt/private
LIBPRIDIR_WIN  :=  $(ROOTDIR_WIN)\prebuilt\private
LIBSTDDIR      :=  $(ROOTDIR)/prebuilt/standard
LIBBTCON_WIN   :=  $(ROOTDIR_WIN)\lib\btcon

LIBBTCON          =    btcon
LIBBTCON_SHARE    =    btcon_share
LIBBTHOST         =    bthost
LIBCHIPDRV        =    chipdrv
LIBDSPALG         =    dspalg
LIBDSPFW          =    dspfw
LIBSTREAM         =    stream
LIBSYSTEM         =    system
LIBPRO            =    profiles
LIBTHIRDPARTY     =    thirdparty
LIBMIDDLEWARE     =    middleware

LIBMID           :=
LIBMID           +=  $(LIBBTHOST)
LIBMID           +=  $(LIBDSPALG)
LIBMID           +=  $(LIBDSPFW)
LIBMID           +=  $(LIBSTREAM)
LIBMID           +=  $(LIBPRO)
LIBMID           +=  $(LIBTHIRDPARTY)
LIBMID           +=  $(LIBMIDDLEWARE)

LIBSYS           :=
LIBSYS           +=  $(LIBCHIPDRV)
LIBSYS           +=  $(LIBSYSTEM)
################################################################################
# Make and Include Folder Configuration
################################################################################
SUBDIR         :=
SUBDIR         +=  $(wildcard $(APPSRCDIR))
SUBDIR         +=  $(wildcard $(APPSRCDIR)/*)
SUBDIR         +=  $(wildcard $(APPSRCDIR)/*/*)

SUBDIR         +=  $(wildcard $(SRCDIR))
SUBDIR         +=  $(wildcard $(SRCDIR)/*)

INCDIR         :=
INCDIR         +=  $(wildcard $(PUBDIR))
INCDIR         +=  $(wildcard $(PUBDIR)/*)

INCDIR         +=  $(wildcard $(SRCDIR))
INCDIR         +=  $(wildcard $(SRCDIR)/*)
INCDIR         +=  $(wildcard $(SRCDIR)/*/*)

INCDIR         +=  $(wildcard $(APPSRCDIR))
INCDIR         +=  $(wildcard $(APPSRCDIR)/*)
INCDIR         +=  $(wildcard $(APPSRCDIR)/*/*)

################################################################################
# Pattern List
################################################################################
ASM_SRC      =
C_SRC        =
OBJ          =  $(ASM_SRC:%.S=$(OUTDIR)/%.o) $(C_SRC:%.c=$(OUTDIR)/%.o)
DEP          =  $(ASM_SRC:%.S=$(OUTDIR)/%.d) $(C_SRC:%.c=$(OUTDIR)/%.d)

################################################################################
# Make and Include Folder Configuration
################################################################################
-include    $(SUBDIR:%=%/$(FEATURE))
-include    $(SUBDIR:%=%/.define.mak)
-include    $(SUBDIR:%=%/.source_app.mak)
-include    $(SUBDIR:%=%/.source.mak)
-include    $(INCDIR:%=%/.include.mak)

################################################################################
# Link and Compile Configuration
################################################################################
LIB         :=
#LIB         +=  c

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
ASFLAG      +=  -ffunction-sections
#ASFLAG      +=  -mtext-section-literals
#ASFLAG      +=  --text-section-literals
#ASFLAG      +=  -DRTLSIM

ifeq ($(APP), bootloader)
ASFLAG      +=  -DBOOTLOADER
CCFLAG      +=  -D__EXT_BOOTLOADER__
<<<<<<< HEAD
=======
CCFLAG      +=  -D__UBL__
>>>>>>> db20e11 (second commit)
endif

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
#CCFLAG      +=  -DRTLSIM

ifeq ($(APP), bootloader)
CCFLAG      +=  -DBOOTLOADER
CCFLAG      +=  -D__EXT_BOOTLOADER__
endif

# mbedtls
CCFLAG_1    ?=  -DMBEDTLS_CONFIG_FILE="\"$(APPSRCPATH)/inc/mbedtls-config-AB1562.h\""

# lzma
CCFLAG      +=  -D_7ZIP_ST


LIBGCC      :=  gcc
LIBHAL      :=  hal
LIBC        :=  c
LIBLOADER   :=  loader
LIBBOARD    :=  handlers-board

LDFLAG      +=  --gc-sections
LDFLAG      +=  --size-opt
LDFLAG      +=  --start-group
<<<<<<< HEAD
ifeq ($(MCSYNC_SHARE_MODE),y)
=======
ifeq ($(AIR_MCSYNC_SHARE_MODE_ENABLE),y)
>>>>>>> db20e11 (second commit)
LDFLAG      +=  -L$(LIBMIDDIR) -l$(LIBBTCON_SHARE) $(foreach n,$(LIBMID),-l$(n))
else
LDFLAG      +=  -L$(LIBMIDDIR) -l$(LIBBTCON) $(foreach n,$(LIBMID),-l$(n))
endif
LDFLAG      +=  -L$(LIBSYSDIR) $(foreach n,$(LIBSYS),-l$(n))
LDFLAG      +=  -L$(LIBPRIDIR)
LDFLAG      +=  -lswp
LDFLAG      +=  -lanticopy
<<<<<<< HEAD

ifeq ($(MTK_INEAR_ENHANCEMENT),y)
LDFLAG       +=  -lecnr62_inear
else ifeq ($(MTK_DUALMIC_INEAR),y)
LDFLAG       +=  -lecnr62_dualmic_inear
else ifeq ($(MTK_3RD_PARTY_NR),y)
LDFLAG       +=  -lec_rxnr
LDFLAG       +=  -lfir_eq
=======
LDFLAG      +=  -lsec

ifeq ($(AIR_TXNR_MODE),mode_2)
LDFLAG       +=  -lecnr62_inear
else ifeq ($(AIR_TXNR_MODE),mode_3)
LDFLAG       +=  -lecnr62_dualmic_inear
else ifeq ($(AIR_TXNR_MODE),mode_6)
LDFLAG       +=  -lec_rxnr
LDFLAG       +=  -lfir_eq
else ifeq ($(AIR_TXNR_MODE),mode_4)
LDFLAG       +=  -lec_rxnr
LDFLAG       +=  -lfir_eq
LDFLAG       +=  -ligo_nr_1mic
>>>>>>> db20e11 (second commit)
else
LDFLAG       +=  -lecnr30
endif
LDFLAG       +=  -ldnn_fix
LDFLAG       +=  -lfea_16k
LDFLAG       +=  -lmp3_dec
#LDFLAG       +=  -lcelt_hd.a
LDFLAG       +=  -lopus_enc
LDFLAG       +=  -lSBCdec
LDFLAG       +=  -lAACdec
LDFLAG       +=  -lplc
LDFLAG       +=  -lmsbcdec
LDFLAG       +=  -lmsbcenc
LDFLAG       +=  -lcpd30
LDFLAG       +=  -lpeq
LDFLAG       +=  -lnewpeq
<<<<<<< HEAD
=======
LDFLAG       +=  -lplc1562
>>>>>>> db20e11 (second commit)
LDFLAG       +=  -lcvsd_wb_nb_v1_lib
LDFLAG       +=  -laudio_bist
LDFLAG       +=  -lsampler_by_2
LDFLAG       +=  -lagc30
LDFLAG       +=  -latagc30
LDFLAG       +=  -lafc30
LDFLAG       +=  -latnr30
<<<<<<< HEAD
=======
LDFLAG       +=  -lanc_sigpwr
>>>>>>> db20e11 (second commit)
ifeq ($(AIROHA_WWE_ENABLE),y)
LDFLAG       +=  -lwwe
endif
ifeq ($(AMAZON_AMA_ENABLE),y)
LDFLAG       +=  -lpryon_lite-PRL1000
LDFLAG       +=  -l_xt_dsp_1562
endif
ifeq ($(GOOGLE_GVA_ENABLE),y)
LDFLAG       +=  -lab1562_google_hotword_dsp_api
endif
LDFLAG       +=  -lskew_ctrl
LDFLAG       +=  -lfft
<<<<<<< HEAD

#force touch
LDFLAG       +=  -lforce_det

ifeq ($(MTK_LEAKAGE_DETECTION_ENABLE),y)
LDFLAG       +=  -lleakage_detection
endif
LDFLAG       +=  -lanc_sigpwr
=======
ifeq ($(MTK_LEAKAGE_DETECTION_ENABLE),y)
LDFLAG       +=  -lleakage_detection
endif
>>>>>>> db20e11 (second commit)
LDFLAG       +=  -lgsound

ifeq ($(XIAOAI_DEVELOPMENT),y)
LDFLAG       +=  -lxmauth
endif

LDFLAG      +=  --end-group
LDFLAG      +=  -L$(LIBSTDDIR) -l$(LIBHAL) -l$(LIBC) -l$(LIBGCC) -l$(LIBBOARD)

#LDFLAG      +=  --print-gc-sections
#LDFLAG      +=  --relax
#LDFLAG      +=  --xt-map
<<<<<<< HEAD
=======
#force touch
LDFLAG       +=  -lforce_det
>>>>>>> db20e11 (second commit)

ifeq ($(PROJECT), AB1562)
CCFLAG      +=  -DAB1562=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash.lcf
LDFLAG      +=  --section-start .package=3
endif

ifeq ($(PROJECT), AB1562A)
CCFLAG      +=  -DAB1562A=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash.lcf
LDFLAG      +=  --section-start .package=1
endif

ifeq ($(PROJECT), AB1562M)
CCFLAG      +=  -DAB1562M=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash_4M.lcf
LDFLAG      +=  --section-start .package=6
endif

ifeq ($(PROJECT), AB1561)
CCFLAG      +=  -DAB1561=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash.lcf
LDFLAG      +=  --section-start .package=a
endif

ifeq ($(PROJECT), AB1562F)
CCFLAG      +=  -DAB1562F=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash.lcf
LDFLAG      +=  --section-start .package=2
endif

ifeq ($(PROJECT), AB1562AM)
CCFLAG      +=  -DAB1562AM=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash_4M.lcf
LDFLAG      +=  --section-start .package=4
endif

ifeq ($(PROJECT), AB1562FM)
CCFLAG      +=  -DAB1562F=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash_4M.lcf
LDFLAG      +=  --section-start .package=5
endif

ifeq ($(PROJECT), AB1561M)
CCFLAG      +=  -DAB1562F=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash_4M.lcf
LDFLAG      +=  --section-start .package=b
endif

ifeq ($(PROJECT), AB1561FM)
CCFLAG      +=  -DAB1562F=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash_4M.lcf
LDFLAG      +=  --section-start .package=c
endif

ifeq ($(PROJECT), AB1562U)
CCFLAG      +=  -DAB1562U=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash.lcf
LDFLAG      +=  --section-start .package=d
endif

ifeq ($(PROJECT), AB1563A)
CCFLAG      +=  -DAB1563=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash.lcf
LDFLAG      +=  --section-start .package=7
endif

ifeq ($(PROJECT), AB1563F)
CCFLAG      +=  -DAB1563=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash.lcf
LDFLAG      +=  --section-start .package=8
endif

ifeq ($(PROJECT), AB1563)
CCFLAG      +=  -DAB1563=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_flash.lcf
LDFLAG      +=  --section-start .package=9
endif

ifeq ($(TARGET), LABTEST)
ASFLAG      +=  -DLABTEST
CCFLAG      +=  -DLABTEST=1
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_iram.lcf
LSCRIPT     +=  $(ROOTDIR)/project/hw_sfrB.lcf
CCFLAG      +=  -DSIMULATION=1
endif

ifeq ($(RAM_BOOT), y)
LSCRIPT     :=  $(ROOTDIR)/project/ab1562_iram.lcf
endif
LSCRIPT     +=  $(ROOTDIR)/project/hw_sfrB.lcf

ifeq ($(PLATFORM), FPGA)
CCFLAG      +=  -DFPGA=1
CCFLAG      +=  -DFPGA_ENV
endif

ifeq ($(PLATFORM), ASIC)
CCFLAG      +=  -DASIC=1
endif

ifeq ($(RF), RF1530C)
CCFLAG      +=  -DRF1530C=1
endif

ifeq ($(MODE), RELEASE)
CCFLAG      +=  -DAIROHA_RELEASE
ASFLAG      +=  -DAIROHA_RELEASE
endif

ifeq ($(MODE), DEBUG)
CCFLAG      +=  -DAIROHA_DEBUG
ASFLAG      +=  -DAIROHA_DEBUG
endif

FULLOBJ     :=
FULLOBJ     += $(OBJ)
FULLOBJ     += $(BOOTLOADER)
FULLOBJ     += $(NVKEY_NOFOTA)
FULLOBJ     += $(NVKEY_FOTA)
FULLOBJ     += $(FILES)
FULLOBJ     += $(PIC)

################################################################################
# Make Target
################################################################################
ifeq ($(TARGET), DEBUG)
all : makesub  $(OBJ) $(BOOTLOADER) $(NVKEY_NOFOTA) $(NVKEY_FOTA) NEW_FILES output
else
all : makesub  $(OBJ) output
endif

test :
	@echo $(NVKEY_XML)
	@echo $(NVKEY_FOTA_XML)
	@echo $(SYSKEY_FOTA_XML)
	@echo $(SYSKEY_XML)

clean :
	@-rmdir $(OUTDIR_WIN) /Q /S

makesub :
	@$(CP) $(APPSRCDIR_WIN) $(OUTDIR_WIN)\$(APPSRCFOLDER)
	@$(CP) $(SRCDIR_WIN) $(OUTDIR_WIN)\$(SRCFOLDER)
ifeq "$(wildcard $(OUTPATH_WIN))" ""
	@mkdir $(OUTPATH_WIN)
endif

$(BOOTLOADER) : $(SRCDIR_WIN)\image\AB1562_bootloader.bin
	@xt-objcopy --prefix-sections AB1562_bootloader -I binary -O elf32-xtensa-le -B xtensa $(SRCDIR_WIN)\image\AB1562_bootloader.bin $(OUTDIR)\AB1562_bootloader.o

$(NVKEY_FOTA) : $(ROOTDIR_WIN)\source\NVKEY\$(SYSKEY_FOTA_XML) $(SRCDIR_WIN)\image\$(NVKEY_FOTA_XML)
	@$(ROOTDIR_WIN)\tools\PostBuildTool\PostBuildTool.exe -f nv2bin 40 $(OUTDIR)\nvkey_fota.bin 0 $(ROOTDIR_WIN)\source\NVKEY\$(SYSKEY_FOTA_XML) $(SRCDIR_WIN)\image\$(NVKEY_FOTA_XML)
	@xt-objcopy --prefix-sections nvkey -I binary -O elf32-xtensa-le -B xtensa $(OUTDIR)\nvkey_fota.bin $(OUTDIR)\nvkey_fota_bin.o

$(NVKEY_NOFOTA) : $(ROOTDIR_WIN)\source\NVKEY\$(SYSKEY_XML) $(SRCDIR_WIN)\image\$(NVKEY_XML)
	@$(ROOTDIR_WIN)\tools\PostBuildTool\PostBuildTool.exe -f nv2bin 20 $(OUTDIR)\nvkey_nofota.bin 1 $(ROOTDIR_WIN)\source\NVKEY\$(SYSKEY_XML) $(SRCDIR_WIN)\image\$(NVKEY_XML)
	@xt-objcopy --prefix-sections nvkey -I binary -O elf32-xtensa-le -B xtensa $(OUTDIR)\nvkey_nofota.bin $(OUTDIR)\nvkey_nofota_bin.o

NEW_FILES :
	@$(ROOTDIR_WIN)\tools\PostBuildTool\PostBuildTool.exe -f mediaFiles2bin $(SRCDIR_WIN)\image\$(FILE_XML) $(SRCDIR_WIN)\filesystem 740 $(OUTDIR)\filesystem.bin
	@xt-objcopy --prefix-sections filesystem -I binary -O elf32-xtensa-le -B xtensa $(OUTDIR)\filesystem.bin $(OUTDIR)\filesystem.o

output :
	@echo Linking... $(IMAGE)
ifeq ($(TARGET), LABTEST)
	@$(LD) -o $(IMAGE) $(OBJ) $(PIC) -Map $(MAP) -T $(LSCRIPT) $(LDFLAG)
endif
ifeq ($(TARGET), DEBUG)
	@$(LD) -o $(IMAGE) $(FULLOBJ) -Map $(MAP) -T $(LSCRIPT) $(LDFLAG)
endif
#	@$(NM) --numeric-sort $(IMAGE) > addr.txt
#	@$(NM) --size-sort --reverse-sort $(IMAGE) > size.txt
	@$(OBJDUMP) -d --show-only-insn $(IMAGE) > $(DEASM)
ifeq ($(TARGET), DEBUG)
	@$(OBJCOPY) -O binary $(IMAGE) $(IRAM_BIN) -j .iram.text
	@del /Q $(IRAM_COMP_BIN_W)
	@$(ROOTDIR_WIN)\tools\PostBuildTool\lz4.exe -9 $(IRAM_BIN) $(IRAM_COMP_BIN_W)
	@xt-objcopy --prefix-sections iram_compress -I binary -O elf32-xtensa-le -B xtensa $(IRAM_COMP_BIN) $(OUTDIR)/iram_compress.o
#	@lzma -1 $(IRAM_BIN)
#	@xt-objcopy --prefix-sections iram_compress -I binary -O elf32-xtensa-le -B xtensa $(IRAM_COMP_LZMA) $(OUTDIR)/iram_compress.o
	@$(LD) -o $(IMAGE) $(FULLOBJ) $(IRAM_COMPRESS) -Map $(MAP) -T $(LSCRIPT) $(LDFLAG)
endif
	@$(OBJCOPY) -O binary $(IMAGE) $(BINARY) --remove-section=.log_str --remove-section=.iram.text
	@$(OBJCOPY) -O ihex $(IMAGE) $(HEX)
	@$(OBJCOPY) -O binary $(IMAGE) $(LOG) --only-section=.log_str
#	@$(CPY) $(ROOTDIR_WIN)\out\$(PROJECT)\$(APP)\AB1562.* $(ROOTDIR_WIN)\tools\debug_Environment\AB1562\debug
	@echo Building Completed

################################################################################
# Pattern Rule
################################################################################
$(OUTDIR)/%.o : $(APPSRCPATH)/%.S
	@echo Assembling... $<
	@$(AS) $(ASFLAG) $(INC:%=-I$(SRCPATH)/"%") $(APPINC:%=-I$(APPSRCPATH)/"%") -MD -MF $(subst .o,.d,$@) -c -o $@ $<

$(OUTDIR)/%.o : $(APPSRCPATH)/%.c
	@echo Compiling... $<
#	$(CC) $(CCFLAG) $(INC:%=-I$(SRCPATH)/"%") $(APPINC:%=-I$(APPSRCPATH)/"%") -MD -MF $(subst .o,.d,$@) -c -o $@ $<
	@$(CC) $(CCFLAG) $(CCFLAG_1) $(INC:%=-I$(SRCPATH)/%) $(APPINC:%=-I$(subst ../../project/AB1562,.,$(APPSRCPATH))/%) -MD -MF $(subst .o,.d,$@) -c -o $@ $<

$(OUTDIR)/%.o : $(SRCPATH)/%.S
	@echo Assembling... $<
	@$(AS) $(ASFLAG) $(INC:%=-I$(SRCPATH)/"%") -MD -MF $(subst .o,.d,$@) -c -o $@ $<

$(OUTDIR)/%.o : $(SRCPATH)/%.c
	@echo Compiling... $<
	@$(CC) $(CCFLAG) $(CCFLAG_1) $(INC:%=-I$(SRCPATH)/"%") $(APPINC:%=-I$(subst ../../project/AB1562,.,$(APPSRCPATH))/%) -MD -MF $(subst .o,.d,$@) -c -o $@ $<

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
RM          :=  dtidel
CP          :=  xcopy /T /Y /I
<<<<<<< HEAD
CPY         :=  copy
=======
CPY         :=  copy
>>>>>>> db20e11 (second commit)
