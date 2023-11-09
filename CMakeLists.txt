# cmake版本
cmake_minimum_required(VERSION 3.5)




message("")
message("")

include(${CMAKE_SOURCE_DIR}/cmake/options.cmake)
message("===============编译环境配置===============")
# 设置交叉编译链
set(CROSS_COMPILE_PREFIX arm-none-eabi)
set(CMAKE_C_COMPILER ${CROSS_COMPILE_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${CROSS_COMPILE_PREFIX}-gcc)
set(CMAKE_OBJCOPY ${CROSS_COMPILE_PREFIX}-objcopy)
set(CMAKE_OBJDUMP ${CROSS_COMPILE_PREFIX}-objdump)
set(CMAKE_SIZE ${CROSS_COMPILE_PREFIX}-size)
set(CMAKE_AR ${CROSS_COMPILE_PREFIX}-ar)
message("=================项目配置=================")
# 项目名
project(mkrtos)

#获取源文件根目录
set(root_dir ${CMAKE_SOURCE_DIR})

# 编译参数
set(COM_FLAGS "${MCPU_FLAGS} ${VFP_FLAGS} -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf -mcpu=cortex-m3 -mthumb -nostartfiles  --specs=nosys.specs -std=c11")

#设置编译参数
set(CMAKE_C_FLAGS ${COM_FLAGS})

# 设置链接脚本
set(link_scripts ${CMAKE_SOURCE_DIR}/cmake/rtos.ld)
set(CMAKE_EXE_LINKER_FLAGS "-T ${link_scripts}")

# 支持汇编指令编译
enable_language(ASM)
set(CMAKE_ASM_FLAGS ${COM_FLAGS})

#源文件
include(${CMAKE_SOURCE_DIR}/cmake/include.cmake)
include_directories(${C_INCLUDES})

#编译后输出文件路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output/bin)

message("=================工程编译=================")
#生成运行程序
add_executable(${PROJECT_NAME}.elf ${mcu_asm} ${kernel_code} ${main_src} ${libcpu_qemu_src})

# 生成bin和hex
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.elf ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.elf ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin
)

message("=================编译成功=================")
message("")
message("")
message("=================================================================================")
message("===========                                                           ===========")
message("===========                                                           ===========")
message("===========                    欢迎使用MK_RTOS                        ===========")
message("===========                                                           ===========")
message("===========                                               --板凳      ===========")
message("===========                                                           ===========")
message("================================================================================")


add_custom_target(run
    COMMAND echo -e "================================================================================="
    COMMAND echo -e "===========                                                           ==========="
    COMMAND echo -e "===========                                                           ==========="
    COMMAND echo -e "===========                    欢迎使用MK_RTOS                        ==========="
    COMMAND echo -e "===========                                                           ==========="
    COMMAND echo -e "===========                                               --板凳      ==========="
    COMMAND echo -e "===========                                                           ==========="
    COMMAND echo -e "===========                     系统开始运行！！！                    ==========="
    COMMAND echo -e "===========                                                           ==========="
    COMMAND echo -e "================================================================================="
    COMMAND echo -e ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SOURCE_DIR}/kernel/libcpu/qemu/tools/qemu-system-arm -M lm3s6965evb --kernel ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin -nographic
)

add_custom_target(debug
    COMMAND echo -e "================================================================================="
    COMMAND echo -e "===========                                                           ==========="
    COMMAND echo -e "===========                                                           ==========="
    COMMAND echo -e "===========                    欢迎使用MK_RTOS                        ==========="
    COMMAND echo -e "===========                                                           ==========="
    COMMAND echo -e "===========                                               --板凳      ==========="
    COMMAND echo -e "===========                                                           ==========="
    COMMAND echo -e "===========                     系统开始调试！！！                    ==========="
    COMMAND echo -e "===========                                                           ==========="
    COMMAND echo -e "================================================================================="
    COMMAND echo -e ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SOURCE_DIR}/kernel/libcpu/qemu/tools/qemu-system-arm -M lm3s6965evb --kernel ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin -nographic -s -S
)
