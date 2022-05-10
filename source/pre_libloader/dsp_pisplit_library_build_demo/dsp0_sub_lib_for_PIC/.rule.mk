################################################################################
# Compiler Toolchain Settings
################################################################################
#Xtensa tool chain path & license setting,
#These settings can be configured either in a project's Makefile for the specific
#project or setting here for all the projets.
XTENSA_ROOT ?= /mtkeda/xtensa/Xplorer-7.0.7
XTENSA_VER ?= RG-2017.7-linux
ifeq ($(shell domainname), mcdswrd)
<<<<<<< HEAD
XTENSA_LICENSE_FILE ?= 7400@172.26.66.32
=======
XTENSA_LICENSE_FILE ?= 7400@10.19.50.25
>>>>>>> db20e11 (second commit)
else
XTENSA_LICENSE_FILE ?= 7400@mtklc17
endif

export LM_LICENSE_FILE := $(XTENSA_LICENSE_FILE)
export PATH := $(XTENSA_ROOT)/XtDevTools/install/tools/$(XTENSA_VER)/XtensaTools/bin:$(PATH)
export XTENSA_SYSTEM := $(XTENSA_ROOT)/XtDevTools/XtensaRegistry/$(XTENSA_VER)
export XTENSA_CORE := $(XTENSA_CORE)
LM_LICENSE_FILE := $(strip $(LM_LICENSE_FILE))
XTENSA_CORE     := $(strip $(XTENSA_CORE))
XTENSA_SYSTEM   := $(strip $(XTENSA_SYSTEM))

.PHONY: clean_log

#include $(ROOTDIR)/middleware/verno/module.mk

################################################################################
# Common Rule
################################################################################
clean_log:
ifeq ($(TARGET_PATH),)
	@if [ -e "$(strip $(LOGDIR))" ]; then rm -rf "$(strip $(LOGDIR))"; fi
	@mkdir -p "$(strip $(LOGDIR))"
else
	@echo "trigger by build.sh, skip clean_log"
endif

################################################################################
# Pattern Rule
################################################################################
$(OUTDIR)/%.o : $(ROOTDIR)/%.S
	@echo Assembling... $^
	@mkdir -p $(dir $@)
	@echo $(AS) $(ASFLAG) $(INC:%=-I"$(ROOTDIR)/%") -MD -MF $(subst .o,.d,$@) -c -o $@ $< >> $(BUILD_LOG)
	@$(AS) $(ASFLAG) $(INC:%=-I"$(ROOTDIR)/%") -MD -MF $(subst .o,.d,$@) -c -o $@ $< 2>>$(ERR_LOG)

$(OUTDIR)/%.o : $(ROOTDIR)/%.c
	@echo Compiling... $^
	@mkdir -p $(dir $@)
	@echo $(CC) $(CCFLAG) $(INC:%=-I"$(ROOTDIR)/%") $(DEFINE:%=-D%) -MD -MF $(subst .o,.d,$@) -c -o $@ $< >> $(BUILD_LOG)
	@$(CC) $(CCFLAG) $(INC:%=-I"$(ROOTDIR)/%") $(DEFINE:%=-D%) -MD -MF $(subst .o,.d,$@) -c -o $@ $< 2>>$(ERR_LOG)

$(TARGET_LIB).a: $(OBJ)
	@echo Gen $(TARGET_LIB).a
	@echo Gen $(TARGET_LIB).a >>$(BUILD_LOG)
	@if [ -e "$@" ]; then rm -f "$@"; fi
	@mkdir -p $(dir $@)
	@$(AR) -r $@ $(OBJ) >>$(BUILD_LOG) 2>>$(ERR_LOG);  \
	if [ "$$?" != "0" ]; then \
		echo MODULE BUILD $@ FAIL ; \
		echo MODULE BUILD $@ FAIL >> $(BUILD_LOG) ; \
		exit 1 ;\
	else \
		echo MODULE BUILD $@ PASS ; \
		echo MODULE BUILD $@ PASS >> $(BUILD_LOG) ; \
	fi;


