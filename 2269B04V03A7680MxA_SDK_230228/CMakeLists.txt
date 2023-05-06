
cmake_minimum_required(VERSION 3.10)

INCLUDE("${CMAKE_SOURCE_DIR}/config/Config_APP.cmake")
INCLUDE("${KERNEL_DIR}/${SCMODULE}/KernelDefinitions.cmake")

SET(GCC_STDLIB_TARGET_PATH thumb/v7/nofp)
SET(GCC_VERSION 9.2.1)
SET(STDLIB
" \
    ${GCC_ARM_TOOLCHAIN_DIR}/arm-none-eabi/lib/${GCC_STDLIB_TARGET_PATH}/libm.a \
    ${GCC_ARM_TOOLCHAIN_DIR}/arm-none-eabi/lib/${GCC_STDLIB_TARGET_PATH}/libc.a \
    ${GCC_ARM_TOOLCHAIN_DIR}/arm-none-eabi/lib/${GCC_STDLIB_TARGET_PATH}/libstdc++.a \
    ${GCC_ARM_TOOLCHAIN_DIR}/arm-none-eabi/lib/${GCC_STDLIB_TARGET_PATH}/libg.a \
    ${GCC_ARM_TOOLCHAIN_DIR}/arm-none-eabi/lib/${GCC_STDLIB_TARGET_PATH}/libnosys.a \
    ${GCC_ARM_TOOLCHAIN_DIR}/lib/gcc/arm-none-eabi/${GCC_VERSION}/${GCC_STDLIB_TARGET_PATH}/libgcc.a"
)

#支持的编程语言配置
enable_language(ASM)

#项目名称
PROJECT(userspace)

# 项目配置
INCLUDE("${CMAKE_SOURCE_DIR}/config/buildOptions.cmake")
configure_file(
    "${PROJECT_SOURCE_DIR}/config/userspaceConfig.h.in"
    "${PROJECT_SOURCE_DIR}/config/userspaceConfig.h"
)

# 预定义
ADD_DEFINITIONS(-DHOST_NAME="${SCMODULE}")

if(DEFINED SDK_ADD_PRIV_PREMACRO)
	ADD_DEFINITIONS(${SDK_ADD_PRIV_PREMACRO})
endif()
if(DEFINED SDK_KERNEL_PREMACRO)
	ADD_DEFINITIONS(${SDK_KERNEL_PREMACRO})
endif()


# 编译子目录
if(NOT DEFINED SIMCOM_SDK)
    if(DEFINED QL)
        add_subdirectory(./ql_lib ql_lib)
    else()
        add_subdirectory(./sc_lib sc_lib)
    endif()

    if(HAS_ZLIB)
        add_subdirectory(./public_libs/zlib zlib)
    endif(HAS_ZLIB)
    if(HAS_MIRACL)
        add_subdirectory(./public_libs/miracl miracl)
    endif(HAS_MIRACL)
    if(HAS_SM2)
        add_subdirectory(./public_libs/sm2 sm2)
    endif(HAS_SM2)
endif()

if(HAS_DEMO)
    if(DEFINED QL)
        add_subdirectory(./ql_demo ql_demo)
    else()
        add_subdirectory(./my_demo my_demo)
    endif()
endif(HAS_DEMO)
# 外部库列表
add_library(third_libs OBJECT IMPORTED GLOBAL)

if(DEFINED THIRD_LIB)
set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
    ${THIRD_LIB}
)
endif()

if(DEFINED SIMCOM_SDK)
    if(DEFINED QL)
        set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
            "${CMAKE_SOURCE_DIR}/ql_lib/${SCMODULE}/lib/libql_lib.a"
        )
    else(DEFINED QL)
        set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
            "${CMAKE_SOURCE_DIR}/sc_lib/${SCMODULE}/lib/libsc_lib.a"
        )
    endif(DEFINED QL)

    if(HAS_ZLIB)
        set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
        "${CMAKE_SOURCE_DIR}/public_libs/lib/libzlib.a"
        )
    endif(HAS_ZLIB)
    if(HAS_MIRACL)
        set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
        "${CMAKE_SOURCE_DIR}/public_libs/lib/libmiracl.a"
        )
    endif(HAS_MIRACL)
    if(HAS_SM2)
        set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
        "${CMAKE_SOURCE_DIR}/public_libs/lib/libsm2.a"
        )
    endif(HAS_SM2)
endif(DEFINED SIMCOM_SDK)

# app源码及编译目标文件定义
# AUX_SOURCE_DIRECTORY(./ app_src)
if(DEFINED QL)
SET(app_src ql_application.c)
INCLUDE_DIRECTORIES(
    ./config
    ./ql_lib/inc
    ./ql_lib/${SCMODULE}/inc)
if(DEFINED SIMCOM_SDK)
    INCLUDE_DIRECTORIES(
        ./ql_lib/inc
        ./ql_lib/inc/lwip
        ./ql_lib/inc/lwip/arch
        ./ql_lib/inc/lwip/ipv4
        ./ql_lib/inc/lwip/ipv6
        ./ql_lib/inc/lwip/lwip
        ./ql_lib/inc/lwip/netif
    )
endif()
else()
SET(app_src sc_application.c)
INCLUDE_DIRECTORIES(
    ./config
    ./sc_lib/inc
    ./sc_lib/${SCMODULE}/inc
)
if(DEFINED SIMCOM_SDK)
    INCLUDE_DIRECTORIES(
        ./sc_lib/inc
        ./sc_lib/inc/lwip
        ./sc_lib/inc/lwip/arch
        ./sc_lib/inc/lwip/ipv4
        ./sc_lib/inc/lwip/ipv6
        ./sc_lib/inc/lwip/lwip
        ./sc_lib/inc/lwip/netif
    )
endif()
endif()
add_executable(userspace ${app_src} $<TARGET_OBJECTS:third_libs>)
target_compile_options(userspace PUBLIC -Wall)
set_target_properties(userspace PROPERTIES SUFFIX ".elf")
set_target_properties(userspace PROPERTIES OUTPUT_NAME ${APP_NAME})


if(DEFINED QL)
    if(NOT DEFINED SIMCOM_SDK)
        target_link_libraries(userspace PRIVATE ql_lib)
    endif()
else(DEFINED QL)
    if(NOT DEFINED SIMCOM_SDK)
        target_link_libraries(userspace PRIVATE sc_lib)
    endif()
endif(DEFINED QL)

if(HAS_DEMO)
    if(DEFINED QL)
        target_link_libraries(userspace PRIVATE ql_demo)
    else(DEFINED QL)
        target_link_libraries(userspace PRIVATE my_demo)
    endif(DEFINED QL)
endif(HAS_DEMO)

if(NOT DEFINED SIMCOM_SDK)
    if(HAS_ZLIB)
        target_link_libraries(userspace PRIVATE zlib)
    endif(HAS_ZLIB)
    if(HAS_MIRACL)
        target_link_libraries(userspace PRIVATE miracl)
    endif(HAS_MIRACL)
    if(HAS_SM2)
        target_link_libraries(userspace PRIVATE sm2)
    endif(HAS_SM2)
endif(NOT DEFINED SIMCOM_SDK)

SET(SDK_ADD_PRIV_WRAPFLAGS
    --wrap=fopen
    --wrap=fclose
    --wrap=fread
    --wrap=fwrite
    --wrap=fseek
    --wrap=ferror
    --wrap=feof
    --wrap=ftell
    --wrap=rewind
    --wrap=clearerr
    --wrap=remove
    --wrap=rename
    --wrap=mkdir
)

# 设置连接脚本
SET(LINK_SCRIP "${CMAKE_CURRENT_SOURCE_DIR}/config/${SCMODULE}_linkscript.ld")

TARGET_LINK_OPTIONS(userspace PRIVATE
    -gc-sections -T${LINK_SCRIP}
    -NOSTDLIB ${SDK_ADD_PRIV_WRAPFLAGS}
)


# 库文件输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


# 自定义命令
# add_custom_command(
#     TARGET userspace
#     POST_BUILD
#     DEPENDS userspace
#     COMMAND ${OBJCPY}
#     ARGS -O binary ${CMAKE_BINARY_DIR}/${APP_NAME}.elf ${CMAKE_BINARY_DIR}/${APP_NAME}.bin
# )

if(NOT DEFINED SIMCOM_SDK)
# 安装SDK包
SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})  # default is /usr/local
INSTALL(DIRECTORY config examples
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}
    PATTERN "*.gitignore*"  EXCLUDE
)

INSTALL(DIRECTORY ${TOOL_DIR}/linux ${TOOL_DIR}/win32
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}/tools
    PATTERN "*/cmake" EXCLUDE
    PATTERN "*/cross_tool" EXCLUDE
)
INSTALL(FILES ${TOOL_DIR}/script/env.mak ${TOOL_DIR}/script/make_image_16xx_settings.mak ${TOOL_DIR}/script/make_image_16xx.mak ${TOOL_DIR}/script/make_image_18xx.mak
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}/tools/script
)

if(DEFINED QL)
INSTALL(DIRECTORY ql_demo ql_lib
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}
    PATTERN "ql_lib/src"  EXCLUDE
    PATTERN "*.gitignore*"  EXCLUDE
)
INSTALL(DIRECTORY ql_lib/inc ql_lib/${SCMODULE}
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}/ql_lib
    PATTERN "ql_lib/${SCMODULE}/src"  EXCLUDE
    PATTERN "*.gitignore*"  EXCLUDE
)
INSTALL(FILES ql_application.c
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}
)
INSTALL(TARGETS ql_lib
    ARCHIVE DESTINATION ${ROOT_DIR}/release/${SCMODULE}/ql_lib/${SCMODULE}/lib
)
else()
INSTALL(DIRECTORY sc_demo
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}
    PATTERN "*.gitignore*"  EXCLUDE
)
INSTALL(DIRECTORY sc_lib/inc sc_lib/${SCMODULE}
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}/sc_lib
    PATTERN "sc_lib/${SCMODULE}/src"  EXCLUDE
    PATTERN "*.gitignore*"  EXCLUDE
)
INSTALL(DIRECTORY public_libs
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}
    PATTERN "*.c"  EXCLUDE
    PATTERN "*.txt"  EXCLUDE
)
INSTALL(FILES sc_application.c
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}
)
INSTALL(TARGETS sc_lib
    ARCHIVE DESTINATION ${ROOT_DIR}/release/${SCMODULE}/sc_lib/${SCMODULE}/lib
)
endif()

INSTALL(DIRECTORY ${KERNEL_DIR}/${SCMODULE}
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}/kernel
)

INSTALL(FILES CMakeLists.txt app_build_doc.md makeDepend.mak ${ROOT_DIR}/Makefile
    DESTINATION ${ROOT_DIR}/release/${SCMODULE}
)

endif()
