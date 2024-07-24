set(libcpu_head "mkrtos/libcpu/qemu")

set(kernel_head "mkrtos/kernel/include"
                "mkrtos/kernel/tools/include"
)
set(mklib_head "mkrtos/mklib/include")

# qemu
set(mcu_asm "mkrtos/libcpu/qemu/cm3_gcc.s")

aux_source_directory("app" main_src)

aux_source_directory("mkrtos/kernel/src" mkrtos_kernel_src)
aux_source_directory("mkrtos/mklib/src" mkrtos_mklib_src)
aux_source_directory("mkrtos/libcpu/qemu" mkrtos_libcpu_qemu)

set(kernel_code ${mkrtos_kernel_src} ${mkrtos_mklib_src})

set(C_INCLUDES 
    ${libcpu_head}
    ${kernel_head}
    ${mklib_head}
)


