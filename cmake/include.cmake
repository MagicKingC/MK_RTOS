set(libcpu_head "kernel/libcpu/qemu")
set(kernel_head "kernel/code/include"
"kernel/code/tools/include"
)
# qemu
set(mcu_asm "kernel/libcpu/qemu/mcu_cm3.s"
)


aux_source_directory("app" main_src)
aux_source_directory("kernel/libcpu/qemu" libcpu_qemu_src)

aux_source_directory("kernel/code/tools/src" kernel_code_tools)
aux_source_directory("kernel/code/src" kernel_code_src)

set(kernel_code ${kernel_code_tools} ${kernel_code_src})

set(C_INCLUDES 
    ${libcpu_head}
    ${kernel_head}
)


