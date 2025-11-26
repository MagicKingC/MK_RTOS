set(libcpu_head "mkrtos/libcpu/m4")

set(kernel_head "mkrtos/kernel/include"
                "mkrtos/kernel/tools/include"
                "mkrtos"
)
set(mklib_head "mkrtos/mklib/include")

# m4
set(mcu_asm "mkrtos/libcpu/m4/cm4_gcc.s")

aux_source_directory("app" main_src)

aux_source_directory("mkrtos/kernel/src" mkrtos_kernel_src)
aux_source_directory("mkrtos/mklib/src" mkrtos_mklib_src)
aux_source_directory("mkrtos/libcpu/m4" mkrtos_libcpu_m4)

set(kernel_code ${mkrtos_kernel_src} ${mkrtos_mklib_src})

set(C_INCLUDES 
    ${libcpu_head}
    ${kernel_head}
    ${mklib_head}
)


