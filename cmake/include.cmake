set(libcpu_head "mk-rtos/libcpu/qemu")
set(kernel_head "mk-rtos/kernel/inc"
                "mk-rtos/tools/inc"
                "app"
)
# qemu
set(mcu_asm "mk-rtos/libcpu/qemu/mcu_cm3.s"
)


aux_source_directory("app" main_src)
aux_source_directory("mk-rtos/libcpu/qemu" libcpu_qemu_src)

aux_source_directory("mk-rtos/tools/src" mk_rtos_tools_src)
aux_source_directory("mk-rtos/kernel/src" mk_rtos_kernel_src)

set(kernel_code ${mk_rtos_tools_src} ${mk_rtos_kernel_src})

set(C_INCLUDES 
    ${libcpu_head}
    ${kernel_head}
)


