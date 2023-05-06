#
# Makefile
#

CACHE_FILE = cache.mak

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT_DIR := $(patsubst %/,%, $(dir $(mkfile_path)))

-include ${CACHE_FILE}

ifneq (,${GIT_ID})
GIT_ID_CACHE := YES
endif
ifneq (,${TOOL_DIR})
TOOL_DIR_CACHE := YES
endif
ifneq (,${APP_DIR})
APP_DIR_CACHE := YES
endif
ifneq (,${PREBUILD_DIR})
PREBUILD_DIR_CACHE := YES
endif

ifeq (kernel, $(notdir $(wildcard $(ROOT_DIR)/kernel)))
SIMCOM_RELEASE_SDK := YES
SIMCOM_SDK_MACRO := $(strip -DSIMCOM_SDK=ON)
TOOL_DIR := $(ROOT_DIR)/tools
APP_DIR := $(ROOT_DIR)
KERNEL_DIR :=$(ROOT_DIR)/kernel
else
SIMCOM_RELEASE_SDK :=
SIMCOM_SDK_MACRO :=
KERNEL_DIR :=$(ROOT_DIR)/tavor/Arbel/kernel
TOOL_DIR ?= $(ROOT_DIR)/tavor/tools
APP_DIR ?= $(ROOT_DIR)/simcom/userspace
PREBUILD_DIR ?= $(ROOT_DIR)/tavor/Arbel/prebuild_images
endif

OUT = out
RELEASE = release
RELEASE_SET = SIMCOM_SDK_SET
MAKEFILENAME := Makefile
CRC_SET := crc_set
OBJCPY := arm-none-eabi-objcopy
API_MAP_FILE_NAME := api_map
APP_NAME := customer_app

KERNEL_LINK_DEFINE := kernel_link_define.mak

-include ${TOOL_DIR}/script/env.mak

SC_TOOLCHAIN_DIR := ${TOOL_DIR}/${PLATFORM}
CROSS_TOOL_DIR := ${TOOL_DIR}/${PLATFORM}/cross_tool
GCC_ARM_TOOLCHAIN_DIR := ${CROSS_TOOL_DIR}/gcc-arm-none-eabi
GCC_ARM_TOOLCHAIN := ${GCC_ARM_TOOLCHAIN_DIR}/bin

API_MAP_MAKEFILE := ${ROOT_DIR}/simcom/config/apiMapInit.mak
API_MAP_INIT_SCRIPT := ${TOOL_DIR}/script/sapiInit.py
TARGET_IMAGE_MAKEFILE := $(TOOL_DIR)/script/make_image_16xx.mak

export PATH := ${PATH}$(GOOD_BREAKER)$(subst $(BAD_SLASH),$(GOOD_SLASH),${TOOL_DIR}/${PLATFORM})$(GOOD_BREAKER)$(subst $(BAD_SLASH),$(GOOD_SLASH),${GCC_ARM_TOOLCHAIN})


-include ${ROOT_DIR}/simcom/config/custom_list.mak
-include ${APP_DIR}/makeDepend.mak

MAKEFILE_DEPEND = $(patsubst %,${APP_DIR}/%,${MAKEFILE_DEPEND_LIST})

ifneq (,$(SIMCOM_RELEASE_SDK))
SCMODULE := $(sort $(notdir $(wildcard $(KERNEL_DIR)/*)))
else
SCMODULE := $(sort $(notdir $(wildcard ${ROOT_DIR}/simcom/config/config_*.mak)))
SCMODULE := ${patsubst config_%.mak,%,${SCMODULE}}
endif

.PHONY:info info_pre cleanAll clean clean_all_app clean_all_kernel

info: info_pre info_modules info_end

all:info-all
info-all: info_pre info_modules_custom info_end

info_end:
	@echo -
	@echo -  clean list:
	@echo -      clean                 [clean all modules]
ifneq (,$(SIMCOM_RELEASE_SDK))
	@echo -      clean_[module]        [clean a module]
else
	@echo -      clean_app             [clean all app, target and object files]
	@echo -      clean_kernel          [clean all kernel, just target files]
	@echo -      clean_[module]        [clean app and kernel, just kernel target files]
	@echo -      clean_[module]_app    [target and object files]
	@echo -      clean_[module]_kernel [just target files]
endif
ifneq (,$(SIMCOM_RELEASE_SDK))
else
	@echo -
	@echo -  install list:
	@echo -      install
	@echo -      install_set
	@echo -      install_[module]
	@echo -      uninstall
	@echo -      uninstall_set
	@echo -      uninstall_[module]
endif
	@echo -
	@echo ---------------------------------------------

info_pre:
	@echo ---------------------------------------------
	@echo -
ifneq (,$(SIMCOM_RELEASE_SDK))
	@echo -  build method: gnumake [target]
	@echo -
	@echo -  target:[module list],[clean list]
else
	@echo -  build method: gnumake [target] (GIT_ID=[work id]) (PREBUILD_DIR=[dir]) (TOOL_DIR=[dir]) (APP_DIR=[dir]) (HOLD=[something])
	@echo -
	@echo -  target:[module](_[custom])(_app/_kernel/_force/_remake),[clean list],[install list],upgrade
	@echo -
	@echo -  GIT_ID is used to dowload userspace code and tools automatically.
	@echo -  PREBUILD_DIR is set the prebuild images dir, defulte is $$(ROOT_DIR)/tavor/Arbel/prebuild_images
	@echo -  TOOL_DIR is set the tools dir, defulte is $$(ROOT_DIR)/tavor/tools
	@echo -  APP_DIR is set the userspace dir, defulte is $$(ROOT_DIR)/simcom/userspace
	@echo -  HOLD is used to force forbid compiling cp.
	@echo -
	@echo -  show customs of the modules: gnumake [module]-list
	@echo -  show all modules and customs at onece: gnumake all/info-all
	@echo -
	@echo -  upgrade:
	@echo -    Do git pull --rebase for all. If the sub-warehouse has not cloned yet, it will clone it automatically.
endif
	@echo -
	@echo -  module list:

info_modules:

info_modules_custom:


################################
#    git pull all
################################
ifeq (,$(SIMCOM_RELEASE_SDK))

.PHONY: upgrade upgrade_base upgrade_tools upgrade_app upgrade_prebuild create_cache

upgrade:create_cache upgrade_base upgrade_tools upgrade_app upgrade_prebuild

upgrade_base:
	@echo ----------------------------
	@echo ----- uprade base code -----
	@echo ----------------------------
	-git pull --rebase

upgrade_tools:${TOOL_DIR}
	@echo ------------------------
	@echo ----- uprade tools -----
	@echo ------------------------
	-$(word 1,$(subst :, ,$<)): && cd $< && git pull --rebase

upgrade_app:${APP_DIR}
	@echo ----------------------------
	@echo ----- uprade userspace -----
	@echo ----------------------------
	-$(word 1,$(subst :, ,$<)): && cd $< && git pull --rebase

upgrade_prebuild:${PREBUILD_DIR}
	@echo ----------------------------------
	@echo ----- uprade prebuild images -----
	@echo ----------------------------------
	-$(word 1,$(subst :, ,$<)): && cd $< && git pull --rebase

create_cache:
ifneq (,${GIT_ID_CACHE}${TOOL_DIR_CACHE}${APP_DIR_CACHE}${PREBUILD_DIR_CACHE})
	echo ###########################> ${CACHE_FILE}
	echo #   makefile cache data   #>> ${CACHE_FILE}
	echo ###########################>> ${CACHE_FILE}
ifneq (,${GIT_ID_CACHE})
	echo GIT_ID ?= ${GIT_ID}>> ${CACHE_FILE}
endif
ifneq (,${TOOL_DIR_CACHE})
	echo TOOL_DIR ?= ${TOOL_DIR}>> ${CACHE_FILE}
endif
ifneq (,${APP_DIR_CACHE})
	echo APP_DIR ?= ${APP_DIR}>> ${CACHE_FILE}
endif
ifneq (,${PREBUILD_DIR_CACHE})
	echo PREBUILD_DIR ?= ${PREBUILD_DIR}>> ${CACHE_FILE}
endif
endif

else  # ifeq (,$(SIMCOM_RELEASE_SDK))

create_cache:
upgrade_tools:
upgrade_prebuild:

endif  # ifeq (,$(SIMCOM_RELEASE_SDK))


################################
# tools process
################################
ifeq (,$(SIMCOM_RELEASE_SDK))
${TOOL_DIR}:
	$(word 1,$(subst :, ,$@)): && cd $(subst $(BAD_SLASH),$(GOOD_SLASH),$(dir $@)) && git clone "ssh://${GIT_ID}@10.238.1.10:29418/OpenCpu" -b tools tools
endif

${TOOL_DIR}/${PLATFORM}/cmake.zip:${TOOL_DIR}
${TOOL_DIR}/${PLATFORM}/cross_tool.zip:${TOOL_DIR}
$(API_MAP_INIT_SCRIPT):${TOOL_DIR}
$(TARGET_IMAGE_MAKEFILE):${TOOL_DIR}

ifneq (,${CMAKE_DIR})
${CMAKE_DIR}:${TOOL_DIR}/${PLATFORM}/cmake.zip
	$(word 1,$(subst :, ,$@)): && cd $(dir $@) && $(GZIP) $(GZIPARG) $<
	$(word 1,$(subst :, ,$@)): && cd $@ && $(MKDIR) $(MKDIRARG) fuck_windows && $(RMDIR) $(RMDIRARG) fuck_windows
endif

${CROSS_TOOL_DIR}:${TOOL_DIR}/${PLATFORM}/cross_tool.zip
	$(word 1,$(subst :, ,$@)): && cd $(dir $@) && $(GZIP) $(GZIPARG) $<
	$(word 1,$(subst :, ,$@)): && cd $@ && $(MKDIR) $(MKDIRARG) fuck_windows && $(RMDIR) $(RMDIRARG) fuck_windows

# tools process end


################################
# app code process
################################
ifeq (,$(SIMCOM_RELEASE_SDK))
${APP_DIR}:
	$(word 1,$(subst :, ,$@)): && cd $(subst $(BAD_SLASH),$(GOOD_SLASH),$(dir $@)) && git clone "ssh://${GIT_ID}@10.238.1.10:29418/OpenCpu" -b userspace_1.0 userspace
endif

$(APP_DIR)/config/ToolChain.cmake:$(APP_DIR)
$(APP_DIR)/config/Config_APP.cmake:$(APP_DIR)

# app code process end

################################
# prebuild image files process
################################
ifeq (,$(SIMCOM_RELEASE_SDK))
${PREBUILD_DIR}:
	$(word 1,$(subst :, ,$@)): && cd $(subst $(BAD_SLASH),$(GOOD_SLASH),$(dir $@)) && git clone "ssh://${GIT_ID}@10.238.1.10:29418/prebuild" -b images-ASR $(notdir $@)
endif

# prebuild image files process end


################################
# clean option define
################################
clean: clean_app

ifneq (,$(SIMCOM_RELEASE_SDK))
else
clean: clean_kernel cleanAll clean_file

clean_file:

cleanAll:
	cd ./tavor/Arbel/build && cleanAll.bat

clean_kernel:
	-$(RMDIR) $(RMDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(KERNEL_DIR))
	cd $(ROOT_DIR)/tavor/Arbel/bin && $(RM) $(RMARG) *

uninstall:
	-$(RMDIR) $(RMDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE))

endif

clean_app:
	-$(RMDIR) $(RMDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/$(OUT))

# clean option define end


################################
# target create
################################
define user_option_process

.PHONY:clean_$(1)$(2)$(3)$(4) clean_$(1)$(2)$(3)$(4)_app clean_$(1)$(2)$(3)$(4)_kernel $(1)$(2)$(3)$(4) $(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(APP_NAME).elf

################################
## info create
################################
ifeq (,$(4))
info_modules:$(1)$(2)$(3)$(4)_info
endif
info_modules_custom:$(1)$(2)$(3)$(4)_info

$(1)$(2)$(3)$(4)_info:
	@echo -      $(1)$(2)$(3)$(4)

ifneq (,$(4))
$(1)$(2)$(3)-list:$(1)$(2)$(3)-list_s $(patsubst _%,%,$(4))_info
else
$(1)$(2)$(3)-list:$(1)$(2)$(3)-list_s
	@echo -
	@echo ---------------------------------------------

$(1)$(2)$(3)-list_s:
	@echo ---------------------------------------------
	@echo -
	@echo -  $(1)$(2)$(3) custom list:
endif
## info create end


$(1)$(2)$(3)$(4)_app:
	$(MAKE) clean_$(1)$(2)$(3)$(4)_app
	$(MAKE) $(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(APP_NAME).bin
	@echo ---------------------------
	@echo ----- build success!! -----
	@echo ---------------------------

$(1)$(2)$(3)$(4)_kernel:
	$(MAKE) clean_$(1)$(2)$(3)$(4)_kernel
	$(MAKE) $(KERNEL_DIR)/$(1)$(2)$(3)$(4)/cp.bin
	@echo ---------------------------
	@echo ----- build success!! -----
	@echo ---------------------------

$(1)$(2)$(3)$(4)_force:
	$(MAKE) clean_$(1)$(2)$(3)$(4)_app
	$(MAKE) clean_$(1)$(2)$(3)$(4)_kernel
	$(MAKE) $(1)$(2)$(3)$(4)

$(1)$(2)$(3)$(4)_remake:
	$(MAKE) clean_$(1)$(2)$(3)$(4)
	$(MAKE) cleanAll
	$(MAKE) $(1)$(2)$(3)$(4)

$(1)$(2)$(3)$(4):create_cache upgrade_tools $(ROOT_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(1)$(2)$(3)$(4).zip
	@echo ---------------------------
	@echo ----- build success!! -----
	@echo ---------------------------


$(ROOT_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(1)$(2)$(3)$(4).zip:$(TARGET_IMAGE_MAKEFILE) $(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(APP_NAME).bin $(KERNEL_DIR)/$(1)$(2)$(3)$(4)/cp.bin ${PREBUILD_DIR} upgrade_prebuild
	@echo --------------------------------
	@echo ----- create aboot package -----
	@echo --------------------------------
	-${RM} ${RMARG} $$(subst $${BAD_SLASH},$${GOOD_SLASH},$$@)
	-${RMDIR} ${RMDIRARG} $$(subst $${BAD_SLASH},$${GOOD_SLASH},$$(subst .zip,_aboot,$$@))
	$(MAKE) -C $$(dir $$<) -f $$(notdir $$<) ROOT_DIR=${ROOT_DIR} KERNEL_DIR=$(KERNEL_DIR) APP_DIR=${APP_DIR} TOOL_DIR=$(TOOL_DIR) PREBUILD_DIR=${PREBUILD_DIR} SC_MODULE_BASE=$(1)$(2)$(3)_OPENSDK SC_HD_CFG=${2} SC_MODULE=$(1) SC_USR_OPT=$(4) SC_MODULE_FULL=$(1)$(2)$(3)$(4) OUT=${OUT} APP_NAME=$(APP_NAME) SIMCOM_RELEASE_SDK=${SIMCOM_RELEASE_SDK} ASR_PLAT=160X
ifneq (,$(SIMCOM_RELEASE_SDK))
else
	-${RMDIR} ${RMDIRARG} $$(subst $${BAD_SLASH},$${GOOD_SLASH},${KERNEL_DIR}/$(1)$(2)$(3)$(4)/$$(subst .zip,_aboot,$$(notdir $$@))/)
	-${MKDIR} ${MKDIRARG} $$(subst $${BAD_SLASH},$${GOOD_SLASH},${KERNEL_DIR}/$(1)$(2)$(3)$(4)/$$(subst .zip,_aboot,$$(notdir $$@))/)
	${COPY} ${COPYARG} $$(subst $${BAD_SLASH},$${GOOD_SLASH},$$(subst .zip,_aboot,$$@)) $$(subst $${BAD_SLASH},$${GOOD_SLASH},${KERNEL_DIR}/$(1)$(2)$(3)$(4)/$$(subst .zip,_aboot,$$(notdir $$@))/)
endif


################################
##  build userspace
################################
$(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(APP_NAME).bin: $(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(APP_NAME).elf
	${OBJCPY} -O binary $$< $$@
	$(CRC_SET) $$@ $$(patsubst %/,%,$$(dir $$@))/$(APP_NAME)_crc.bin
ifeq (win32,$(PLATFORM))
	$(RM) $(RMARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$@)
	$(MOVE) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(patsubst %/,%,$$(dir $$@))/$(APP_NAME)_crc.bin) $$(notdir $$@)
else
	$(RM) $(RMARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$@)
	$(MOVE) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(patsubst %/,%,$$(dir $$@))/$(APP_NAME)_crc.bin) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$@)
endif

$(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(APP_NAME).elf: $(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(MAKEFILENAME) $(APP_DIR)/config/$(1)$(2)$(3)$(4)_linkscript.ld
	@echo ---------------------------
	@echo ----- build userspace -----
	@echo ---------------------------
	$$(word 1,$$(subst :, ,$$@)): && $(BUILD) -C $$(dir $$<)

$(APP_DIR)/config/$(1)$(2)$(3)$(4)_linkscript.ld:$(KERNEL_DIR)/$(1)$(2)$(3)$(4)/${KERNEL_LINK_DEFINE}
	@echo ----------------------------------------
	@echo ----- create userspace link script -----
	@echo ----------------------------------------
	-${RM} ${RMARG} $$(subst ${BAD_SLASH},${GOOD_SLASH},$$@)
	python ${TOOL_DIR}/script/userspace_linkscript_generator.py $$< $$@

$(KERNEL_DIR)/$(1)$(2)$(3)$(4)/${KERNEL_LINK_DEFINE}:$(KERNEL_DIR)/$(1)$(2)$(3)$(4)/KernelDefinitions.cmake

$(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(MAKEFILENAME): $(APP_DIR) $(CMAKE_DIR) ${CROSS_TOOL_DIR} $(APP_DIR)/config/ToolChain.cmake $(APP_DIR)/config/Config_APP.cmake $(KERNEL_DIR)/$(1)$(2)$(3)$(4)/KernelDefinitions.cmake ${MAKEFILE_DEPEND}
	-$(MKDIR) $(MKDIRARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(dir $$@))
ifneq (,$(SIMCOM_RELEASE_SDK))
	$$(CMAKE) -B$$(dir $$@) -DSCMODULE="$(1)" $(SIMCOM_SDK_MACRO) \
		-DKERNEL_DIR="$(KERNEL_DIR)" \
		$(patsubst %,-D%=ON,$(subst _, ,$(1))) \
		-D$(1)=ON \
		-DGCC_ARM_TOOLCHAIN_DIR="$(GCC_ARM_TOOLCHAIN_DIR)" \
		-DCMAKE_TOOLCHAIN_FILE="$(APP_DIR)/config/ToolChain.cmake" \
		-DAPP_NAME="$(APP_NAME)" \
		-G $(BUILD_TYPE) \
		$(APP_DIR)
else
	$$(word 1,$$(subst :, ,$$@)): && cd $$(dir $$@) && $$(CMAKE) \
		-DSCMODULE="$(1)$(2)$(3)$(4)" $(SIMCOM_SDK_MACRO) \
		-DKERNEL_DIR="$(KERNEL_DIR)" \
		-DTOOL_DIR="${TOOL_DIR}" \
		-DROOT_DIR="${ROOT_DIR}" \
		$(patsubst %,-D%=ON,$(subst _, ,$(1)$(2)$(3)$(4))) \
		-D$(1)$(2)$(3)$(4)=ON \
		-DGCC_ARM_TOOLCHAIN_DIR="$(GCC_ARM_TOOLCHAIN_DIR)" \
		-DCMAKE_TOOLCHAIN_FILE="$(APP_DIR)/config/ToolChain.cmake" \
		-DAPP_NAME="$(APP_NAME)" \
		-G $(BUILD_TYPE) \
		$(APP_DIR)
endif


clean_$(1)$(2)$(3)$(4):clean_$(1)$(2)$(3)$(4)_app
clean_$(1)$(2)$(3)$(4)_app:
	-$(RMDIR) $(RMDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4))

#  build userspace end

ifneq (,$(SIMCOM_RELEASE_SDK))
else
################################
##  build kernel
################################
$(KERNEL_DIR)/$(1)$(2)$(3)$(4)/KernelDefinitions.cmake:$(KERNEL_DIR)/$(1)$(2)$(3)$(4)/cp.bin
	-$(RM) $(RMARG) $$(subst ${BAD_SLASH},${GOOD_SLASH},$$@)
	python ${TOOL_DIR}/script/extract_macro_from_buildlog.py $(ROOT_DIR)/tavor/Arbel/build/buildlog.txt yes $$(patsubst %.cmake,%,$$@)
	echo KERNEL_LINK_FILE := $$(word 1,$$(subst :, ,$$@)):$$(shell python $${TOOL_DIR}/script/getInfo.py GET_KERNEL_LINKSCRIPT_FILE $$(ROOT_DIR)/tavor/Arbel/build/buildlog.txt)> $$(dir $$@)${KERNEL_LINK_DEFINE}

$(KERNEL_DIR)/$(1)$(2)$(3)$(4)/cp.bin: tavor/Arbel/bin/$(1)$(2)$(3)$(4)_cp.bin
	-${RMDIR} ${RMDIRARG} $$(dir $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$@))
	-$(MKDIR) $(MKDIRARG) $$(dir $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$@))
	$(COPY_FILE) $(COPYARG_FILE) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(patsubst %/,%,$$(dir $$<))/*.bin) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(dir $$@))
	$(COPY_FILE) $(COPYARG_FILE) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(patsubst %/,%,$$(dir $$<))/*.axf) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(dir $$@))
	$(COPY_FILE) $(COPYARG_FILE) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(patsubst %/,%,$$(dir $$<))/*.map) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(dir $$@))
	$(COPY_FILE) $(COPYARG_FILE) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(patsubst %/,%,$$(dir $$<))/*.txt) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(dir $$@))
	-${RM} ${RMARG} $$(subst ${BAD_SLASH},${GOOD_SLASH},$$(dir $$@)$$(notdir $$<))

tavor/Arbel/bin/$(1)$(2)$(3)$(4)_cp.bin:
	@echo ---------------------------
	@echo -----  build kernel   -----
	@echo ---------------------------
ifeq (,${HOLD})
ifeq (,$(wildcard $(ROOT_DIR)/tavor/Arbel/build/sc_kernel_build_$(1)$(2)$(3)$(4)))
	$(MAKE) cleanAll
endif
	-${RM} ${RMARG} $(subst ${BAD_SLASH},${GOOD_SLASH},$(ROOT_DIR)/tavor/Arbel/build/sc_kernel_build_*)
	cd. > $(subst ${BAD_SLASH},${GOOD_SLASH},$(ROOT_DIR)/tavor/Arbel/build/sc_kernel_build_$(1)$(2)$(3)$(4))
ifneq (,$(findstring 1606,$(3)))
ifeq (TRUE,$(patsubst %S,TRUE,$(2)))
	cd tavor/Arbel/build && module_lteonly_buildcust_8mram_craneL.bat _$(1) _$(patsubst _%,%,$(2)) _$(patsubst _%,%,$(3)) _$(patsubst _%,%,$(4)) _$(1)$(2)$(3)$(4)
else ifeq (TRUE,$(patsubst %V,TRUE,$(2)))
	cd tavor/Arbel/build && module_lteonly_buildcust_4mram_craneL.bat _$(1) _$(patsubst _%,%,$(2)) _$(patsubst _%,%,$(3)) _$(patsubst _%,%,$(4)) _$(1)$(2)$(3)$(4)
endif
else ifneq (,$(findstring 1603,$(3)))
	cd tavor/Arbel/build && module_tigx_buildcust.bat _$(1) _$(patsubst _%,%,$(2)) _$(patsubst _%,%,$(3)) _$(patsubst _%,%,$(4)) _$(1)$(2)$(3)$(4)
endif
endif
	-${RM} ${RMARG} $$(subst ${BAD_SLASH},${GOOD_SLASH},$$@)
ifneq (,$(findstring 1603,$(3)))
	${COPY_FILE} ${COPYARG_FILE} $$(subst ${BAD_SLASH},${GOOD_SLASH},$$(dir $$@)cp_TIGX.bin) $$(subst ${BAD_SLASH},${GOOD_SLASH},$$@)
else
	${COPY_FILE} ${COPYARG_FILE} $$(subst ${BAD_SLASH},${GOOD_SLASH},$$(dir $$@)cp.bin) $$(subst ${BAD_SLASH},${GOOD_SLASH},$$@)
endif


ifneq (,$(findstring _QL_, _$(4)_))
tavor/Arbel/bin/$(1)$(2)$(3)$(4)_cp.bin:$(ROOT_DIR)/quectel/ql_interface/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c $(ROOT_DIR)/quectel/ql_interface/$(1)$(2)$(3)$(4)/inc/$(API_MAP_FILE_NAME).h
$(ROOT_DIR)/quectel/ql_interface/$(1)$(2)$(3)$(4)/inc/$(API_MAP_FILE_NAME).h: $(ROOT_DIR)/quectel/ql_interface/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c
else
tavor/Arbel/bin/$(1)$(2)$(3)$(4)_cp.bin:$(ROOT_DIR)/simcom/sc_interface/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c $(ROOT_DIR)/simcom/sc_interface/$(1)$(2)$(3)$(4)/inc/$(API_MAP_FILE_NAME).h
$(ROOT_DIR)/simcom/sc_interface/$(1)$(2)$(3)$(4)/inc/$(API_MAP_FILE_NAME).h: $(ROOT_DIR)/simcom/sc_interface/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c
endif

ifneq (,$(findstring _QL_, _$(4)_))
$(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(APP_NAME).elf:$(APP_DIR)/ql_lib/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c $(APP_DIR)/ql_lib/$(1)$(2)$(3)$(4)/inc/$(API_MAP_FILE_NAME).h
$(APP_DIR)/ql_lib/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c:$(ROOT_DIR)/quectel/ql_interface/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c
$(APP_DIR)/ql_lib/$(1)$(2)$(3)$(4)/inc/$(API_MAP_FILE_NAME).h:$(ROOT_DIR)/quectel/ql_interface/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c
else
$(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(APP_NAME).elf:$(APP_DIR)/sc_lib/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c $(APP_DIR)/sc_lib/$(1)$(2)$(3)$(4)/inc/$(API_MAP_FILE_NAME).h
$(APP_DIR)/sc_lib/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c:$(ROOT_DIR)/simcom/sc_interface/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c
$(APP_DIR)/sc_lib/$(1)$(2)$(3)$(4)/inc/$(API_MAP_FILE_NAME).h:$(ROOT_DIR)/simcom/sc_interface/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c
endif

## create openSDK api map
ifneq (,$(findstring _QL_, _$(4)_))
$(ROOT_DIR)/quectel/ql_interface/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c:$(API_MAP_MAKEFILE) $(API_MAP_INIT_SCRIPT)
else
$(ROOT_DIR)/simcom/sc_interface/$(1)$(2)$(3)$(4)/src/$(API_MAP_FILE_NAME).c:$(API_MAP_MAKEFILE) $(API_MAP_INIT_SCRIPT)
endif
	-$(RMDIR) $(RMDIRARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(dir $$@))
	$(MKDIR) $(MKDIRARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$(dir $$@))
	$(MAKE) -C $$(dir $$<) -f $$(notdir $$<) API_MAP_FILE_NAME=$(API_MAP_FILE_NAME) ROOT_DIR=$(ROOT_DIR) TOOL_DIR=$(TOOL_DIR) APP_DIR=${APP_DIR} SC_MODULE=$(1) SC_HD_CFG=$(2) SC_HD_OPT=$(3) SC_USR_OPT=$(4) SC_MODULE_FULL=$(1)$(2)$(3)$(4)

#  build kernel end

################################
## kernel clean option define
################################
clean_$(1)$(2)$(3)$(4):clean_$(1)$(2)$(3)$(4)_kernel clean_$(1)$(2)$(3)$(4)_file
clean_$(1)$(2)$(3)$(4)_kernel:clean_$(1)$(2)$(3)$(4)_file
	-$(RMDIR) $(RMDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(KERNEL_DIR)/$(1)$(2)$(3)$(4))

clean_file:clean_$(1)$(2)$(3)$(4)_file
clean_$(1)$(2)$(3)$(4)_file:$(API_MAP_MAKEFILE)
	-$(RMDIR) $(RMDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(KERNEL_DIR)/$(1)$(2)$(3)$(4))
	$(MAKE) -C $$(dir $$<) -f $$(notdir $$<) clean API_MAP_FILE_NAME=$(API_MAP_FILE_NAME) ROOT_DIR=$(ROOT_DIR) TOOL_DIR=$(TOOL_DIR) APP_DIR=${APP_DIR} SC_MODULE=$(1) SC_HD_CFG=$(2) SC_HD_OPT=$(3) SC_USR_OPT=$(4) SC_MODULE_FULL=$(1)$(2)$(3)$(4)

install_$(1)$(2)$(3)$(4):$(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(APP_NAME).bin $(KERNEL_DIR)/$(1)$(2)$(3)$(4)/cp.bin $(ROOT_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/$(1)$(2)$(3)$(4).zip
	$(BUILD) -C $(APP_DIR)/$(OUT)/$(1)$(2)$(3)$(4)/ install
	@echo ------------------------------------------
	@echo     install [$(1)$(2)$(3)$(4)] success!!
	@echo ------------------------------------------

uninstall_$(1)$(2)$(3)$(4):
	-$(RMDIR) $(RMDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(1)$(2)$(3)$(4))

## kernel clean option define end

endif  # ifneq (,$(SIMCOM_RELEASE_SDK))
endef  # define user_option_process

define user_option_pre_process
$(eval $(call user_option_process,$(1),$(2),$(3),))

$(foreach custom,$($(1)$(2)$(3)_USR_OPT_LIST),$(eval $(call user_option_process,$(1),$(2),$(3),_$(custom))))

$(eval $(call get_usr_opt_list,$(1)$(2)$(3)))
endef

define hard_option_process
ifeq (,$($(1)$(2)_HD_OPT_LIST))
$$(eval $$(call user_option_pre_process,$(1),$(2),,))
else
$$(foreach hard_option,$($(1)$(2)_HD_OPT_LIST),$$(eval $$(call user_option_pre_process,$(1),$(2),$$(subst _STANDARD,,_$$(hard_option)),)))
endif
endef

define hard_config_process
$(foreach hard_config,$($(1)_HD_CFG_LIST),$(eval $(call hard_option_process,$(1),_$(hard_config))))
endef

ifneq (,$(SIMCOM_RELEASE_SDK))
$(foreach modules,$(SCMODULE),$(eval $(call user_option_process,$(modules),,,)))
else
# info create
define get_usr_opt_list
ALL_USR_OPT_LIST += $($(1)_USR_OPT_LIST)
endef

define make_custom_info
$(1)_info:
	@echo -      $(1)
endef
# info create end

$(foreach modules,$(SCMODULE),$(eval $(call hard_config_process,$(modules))))

$(foreach custom,$(sort $(ALL_USR_OPT_LIST)),$(eval $(call make_custom_info,$(custom))))
endif




################################################
##   release simcom SDK for multiple modules
################################################

ifneq (,$(SIMCOM_RELEASE_SDK))
else
KERNEL_TARGETLIST := $(sort $(notdir $(wildcard $(KERNEL_DIR)/*)))
APP_TARGETLIST := $(sort $(notdir $(wildcard $(APP_DIR)/$(OUT)/*)))
VALID_TARGETLIST := $(filter $(APP_TARGETLIST),$(KERNEL_TARGETLIST))

define copy_module_object
ifneq (,$(findstring _QL,$(1)))
install_set:$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/ql_lib/lib/$(1)/libql_lib.a
$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/ql_lib/lib/$(1)/libql_lib.a: preinstall_set install_set_ql $(APP_DIR)/$(OUT)/$(1)/lib/libql_lib.a
	-$(MKDIR) $(MKDIRARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/ql_lib/$(1)/inc/)
	-$(MKDIR) $(MKDIRARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/ql_lib/$(1)/lib/)
	$(COPY) $(COPYARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/ql_lib/$(1)/inc) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/ql_lib/$(1)/inc/)
	$(COPY_FILE) $(COPYARG_FILE) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/$(OUT)/$(1)/lib/libql_lib.a) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/ql_lib/$(1)/lib/libql_lib.a)
else
install_set:$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/sc_lib/$(1)/lib/libsc_lib.a
$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/sc_lib/$(1)/lib/libsc_lib.a: preinstall_set install_set_sc $(APP_DIR)/$(OUT)/$(1)/lib/libsc_lib.a
	-$(MKDIR) $(MKDIRARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/sc_lib/${1}/inc)
	-$(MKDIR) $(MKDIRARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/sc_lib/${1}/lib)
	$(COPY) $(COPYARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/sc_lib/$(1)/inc) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/sc_lib/$(1)/inc/)
	$(COPY_FILE) $(COPYARG_FILE) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/$(OUT)/$(1)/lib/libsc_lib.a) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/sc_lib/$(1)/lib/libsc_lib.a)
endif

install_set:$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/kernel/${1}
$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/kernel/${1}:${KERNEL_DIR}/${1}
	@echo ----copy kernel [${1}] object----
	-$(MKDIR) $(MKDIRARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$@)
	$(COPY) $(COPYARG) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$<) $$(subst $(BAD_SLASH),$(GOOD_SLASH),$$@/)
endef

.PHONE: install_set preinstall_set uninstall_set

uninstall_set:
	-$(RMDIR) $(RMDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET))

preinstall_set: uninstall_set
	@echo ------------------------------------------
	@echo KERNEL_TARGETLIST: $(KERNEL_TARGETLIST)
	@echo APP_TARGETLIST: $(APP_TARGETLIST)
	@echo target set list: $(VALID_TARGETLIST)
	@echo ------------------------------------------

install_set_sc:
	@echo ----copy sc source----
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/sc_demo/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/sc_lib/inc)
	$(COPY) $(COPYARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/sc_demo) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/sc_demo/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/sc_application.c) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/)
	$(COPY) $(COPYARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/sc_lib/inc) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/sc_lib/inc/)

install_set_ql:
	@echo ----copy ql source----
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/ql_demo/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/ql_lib/inc)
	$(COPY) $(COPYARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/ql_demo) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/ql_demo/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/ql_application.c) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/)
	$(COPY) $(COPYARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/ql_lib/inc) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/ql_lib/inc/)

install_set:
	@echo ----copy general source----
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/config/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/examples/)
	$(COPY) $(COPYARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/config) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/config/)
	$(COPY) $(COPYARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/examples) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/examples/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/CMakeLists.txt) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/app_build_doc.md) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/makeDepend.mak) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/Makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/)
	@echo ----copy thired lib object----
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/lib/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/miracl/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/sm2/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/zlib/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/zlib/minizip/)
	$(COPY) $(COPYARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/public_libs/lib) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/lib/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/public_libs/miracl/*.h) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/miracl/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/public_libs/sm2/*.h) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/sm2/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/public_libs/zlib/*.h) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/zlib/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(APP_DIR)/public_libs/zlib/minizip/*.h) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/public_libs/zlib/minizip/)
	@echo ----copy tools----
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/tools/linux/aboot/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/tools/win32/7z/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/tools/win32/aboot/)
	-$(MKDIR) $(MKDIRARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/tools/script/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TOOL_DIR)/linux/*) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/tools/linux/)
	$(COPY) $(COPYARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TOOL_DIR)/linux/aboot) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/tools/linux/aboot/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TOOL_DIR)/win32/*) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/tools/win32/)
	$(COPY) $(COPYARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TOOL_DIR)/win32/7z) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/tools/win32/7z/)
	$(COPY) $(COPYARG) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TOOL_DIR)/win32/aboot) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/tools/win32/aboot/)
	$(COPY_FILE) $(COPYARG_FILE) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TOOL_DIR)/script/*.mak) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(ROOT_DIR)/$(RELEASE)/$(RELEASE_SET)/tools/script/)
	@echo ------------------------------------------
	@echo          install set success!!
	@echo ------------------------------------------

$(foreach module,$(VALID_TARGETLIST),$(eval $(call copy_module_object,$(module))))
endif
