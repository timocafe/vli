
########################################################################
#
# Configure Header File Template
#
########################################################################
configure_file (
        "${PROJECT_SOURCE_DIR}/src/vli/config.hpp.in"
        "${PROJECT_BINARY_DIR}/vli/config.hpp"
        )


########################################################################
#
# Configure Header File Template
#
########################################################################

add_library(vli 
            detail/cpu/kernels_cpu_operations.cpp
      )

########################################################################
#
# GPU Kernel Compile
#
########################################################################

if(VLI_COMPILE_GPU)
    CUDA_ADD_LIBRARY(accelerator_gpu  detail/gpu/polynomial_operations.cu)
endif(VLI_COMPILE_GPU)
