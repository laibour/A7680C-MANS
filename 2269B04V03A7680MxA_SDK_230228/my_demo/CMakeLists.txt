cmake_minimum_required(VERSION 3.10)

# AUX_SOURCE_DIRECTORY(./src sc_demo_src)
AUX_SOURCE_DIRECTORY(./src/token sc_demo_src)
AUX_SOURCE_DIRECTORY(./src/utils sc_demo_src)
# list(APPEND sc_demo_src ./src/simcom_demo.c)
list(APPEND sc_demo_src ./src/demo_helloworld.c)

#########################
#       general config
#########################
if(HAS_UART)
    list(APPEND sc_demo_src ./src/cus_uart.c)
endif()

if(HAS_URC)
#		list(APPEND sc_demo_src ./src/cus_urc.c)
endif()

if(HAS_USB)
    list(APPEND sc_demo_src ./src/cus_usb_vcom.c)
endif()

		list(APPEND sc_demo_src ./src/my_task.c)

#########################
#       other libs
#########################
if(HAS_ZLIB)
    list(APPEND sc_demo_src ./src/demo_zlib.c)
endif()

if(HAS_SM2)
    list(APPEND sc_demo_src ./src/demo_sm2.c)
endif()

# Add the static library
add_library(my_demo STATIC ${sc_demo_src})
target_compile_options(my_demo PUBLIC -Wall)
target_include_directories(my_demo PUBLIC
    ./inc
    ${CMAKE_SOURCE_DIR}/sc_lib/inc
    ${CMAKE_SOURCE_DIR}/sc_lib/${SCMODULE}/inc
    ${CMAKE_SOURCE_DIR}/sc_lib/inc/GPIO
    ${CMAKE_SOURCE_DIR}/sc_lib/inc/token
    ${CMAKE_SOURCE_DIR}/sc_lib/inc/utils
    ${CMAKE_SOURCE_DIR}/my_demo/inc
    ${CMAKE_SOURCE_DIR}/config
)

if(HAS_SM2)
    target_include_directories(sc_demo PUBLIC
        ${CMAKE_SOURCE_DIR}/public_libs/sm2
        ${CMAKE_SOURCE_DIR}/public_libs/miracl
    )
endif(HAS_SM2)

if(HAS_ZLIB)
    target_include_directories(sc_demo PUBLIC
        ${CMAKE_SOURCE_DIR}/public_libs/zlib
        ${CMAKE_SOURCE_DIR}/public_libs/zlib/minizip
    )
endif(HAS_ZLIB)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
