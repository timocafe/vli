# Make sure the linker can find the vli library once it is built. 
link_directories (${PROJECT_BINARY_DIR}/src/vli) 

if(VLI_COMPILE_GPU)
    add_definitions(-DVLI_USE_GPU -DVLI_SPLIT_PARAM=${VLI_SPLIT_PARAM} )
    set(LIBRAIRIES vli accelerator_gpu ${CUDA_CUDART_LIBRARY} ${CUDA_LIBRARIES} ${GMP_LIBRARY_CXX})
else(VLI_COMPILE_GPU)
    if(VLI_USE_OPENMP)
        set(LIBRAIRIES vli ${GMP_LIBRARY_CXX})
    endif(VLI_USE_OPENMP)
endif(VLI_COMPILE_GPU)

add_executable(main_ex main.cpp)
target_link_libraries(main_ex ${LIBRAIRIES})
