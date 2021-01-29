cmake_minimum_required (VERSION 2.8)

######################################################################
# Machine Config
######################################################################
set(DEFAULT_BLAS_LAPACK alps)

if(MACHINE_CONFIG)
  if(EXISTS ${MACHINE_CONFIG})
    message(STATUS "Loading config in " ${MACHINE_CONFIG})
    include(${MACHINE_CONFIG})
  else(EXISTS ${MACHINE_CONFIG})
    message(ERROR " Machine config not found!")
  endif(EXISTS ${MACHINE_CONFIG})
endif(MACHINE_CONFIG)
set(MAKE_INSTALL_PREFIX "/opt/vli" CACHE STRING "Install path prefix, prepended onto install directories.")
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
if(NOT MACHINE_CONFIG)
    set(CMAKE_CXX_FLAGS_RELEASE "-Wall -DNDEBUG -O2 -m64 -std=c++11" CACHE STRING "Compiler flags for Release compiles.")
    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -O0 -m64 -std=c++11" CACHE STRING "Compiler flags for Debug compiles.")
endif(NOT MACHINE_CONFIG)


########################################################################
#
# Project and version information
#
########################################################################

project (vli)
set (VLI_VERSION_MAJOR 1)
set (VLI_VERSION_MINOR 0)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
                              ${CMAKE_SOURCE_DIR}/CMake)

#include(clang-format)

########################################################################
#
# Options
#
########################################################################

#set (BOOST_DIR $ENV{BOOST_DIR} CACHE PATH "Path to the Boost installation (or to the Boost source)")
option (VLI_COMPILE_GPU "Compile the GPU enhanced VLI components" OFF)
option (VLI_TESTS "Build the VLI regression tests" OFF)
option (VLI_USE_OPENMP "Build and execute with openmp" ON)
option (VLI_MAIN  "Build and execute the main" ON)
option (VLI_KEPLER "Target Kepler (ON - default) or Fermi (OFF) GPU architecture" ON)
option (VLI_DOCUMENTATION "Use Doxygen to create the HTML/LATEX based API documentation" OFF)

set (VLI_CPU_FREQ "2.5e9" CACHE STRING "Frequency of the CPU (for benchmark timers)")
set (VLI_SPLIT_PARAM "1.0" CACHE STRING "Split param for cpu/gpu mix mode, 1.0 = full gpu")

set(SIZE  128 192 256)
set(ORDER 1 2 3 4 5 6 7 8 9 10 11 12 13 14)
set(VAR   1 2 3)

foreach(size ${SIZE})
    foreach(order ${ORDER})
        foreach(var ${VAR})
            list (APPEND COMPILE_TYPES "((${size},${order},${var}))")
        endforeach(var)
    endforeach(order)
endforeach(size)

STRING(REPLACE ";" "" COMPILE_TYPES "${COMPILE_TYPES}")

set (VLI_COMPILE_TYPES ${COMPILE_TYPES} CACHE STRING "Numbers of bits, Order Poly, number of variables into the polynomials")
#set (VLI_COMPILE_TYPES "((128,1,1));((128,1,2));((128,1,3));((128,1,4));((192,1,1));((192,1,2));((192,1,3));((192,1,4));((256,1,1));((256,1,2));((256,1,3));((256,1,4))" CACHE STRING "Numbers of bits, Order Poly, number of variables into the polynomials")

########################################################################
#
# Find dependencies
#
########################################################################
     find_package(OpenMP)
     if(OPENMP_FOUND)
     add_definitions(${OpenMP_CXX_FLAGS})
     endif()

if (VLI_COMPILE_GPU)
   find_package (CUDA)
   SET(CUDA_PROPAGATE_HOST_FLAGS OFF)
   if (VLI_KEPLER)
       set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} "-O3 -arch=sm_35 -DNDEBUG -DVLI_KEPLER")
   else (VLI_KEPLER)
       set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} "-O3 -arch=sm_21 -DNDEBUG -DVLI_FERMI")
   endif (VLI_KEPLER)
   include_directories(${CUDA_INCLUDE_DIRS})
else (VLI_COMPILE_GPU)
   set (CUDA_LIBRARIES "")
endif (VLI_COMPILE_GPU)

# The tests need the GMP library
#if (VLI_TESTS)
    find_path(GMP_INCLUDE_DIR gmp.h $ENV{HOME}/.local/include)
    find_library(GMP_LIBRARY_CXX NAMES gmpxx PATH $ENV{HOME}/.local/include)
    find_library(GMP_LIBRARY_C NAMES gmp PATH $ENV{HOME}/.local/include)

    if (GMP_INCLUDE_DIR AND GMP_LIBRARY_C AND GMP_LIBRARY_CXX)
       set(GMP_LIBRARY_CXX ${GMP_LIBRARY_CXX} ${GMP_LIBRARY_C})
       set(GMP_FOUND true)
       include_directories(${GMP_INCLUDE_DIR})
    endif (GMP_INCLUDE_DIR AND GMP_LIBRARY_C AND GMP_LIBRARY_CXX)

    if (GMP_FOUND)
      message(STATUS "Found GMP: ${GMP_LIBRARY_CXX} ${GMP_LIBRARY_C}")
    else (GMP_FOUND)
      message(WARNING "Could not find GMP library which are needed for the regression tests -> Deactivating tests.")
      set (VLI_TESTS OFF)
    endif (GMP_FOUND)
#endif (VLI_TESTS)

find_package(Boost ${BOOST_MIN_VERSION} REQUIRED unit_test_framework)
 
#find_package (Boost COMPONENTS unit_test_framework random)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost libraries not found. Please specify location using the BOOST_DIR variable")
endif()

########################################################################
#
# Set internal variables according to the variables in section "options"
#
########################################################################



########################################################################
#
# Compile and prepare library
#
########################################################################
include_directories ("${PROJECT_BINARY_DIR}") 	
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/src/")

add_subdirectory( src/vli/ )


########################################################################
#
# Main Compile
#
########################################################################
if(VLI_MAIN)
    add_subdirectory(main/)
  # add_subdirectory(benchmark/)
endif()

########################################################################
#
# Tests
#
########################################################################

if(VLI_TESTS)
    include(CTest)
    enable_testing() 
    add_subdirectory (regression)
endif(VLI_TESTS)

########################################################################
#
# Documentation
#
########################################################################

if(VLI_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR 
      "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  #-- Configure the Template Doxyfile for our specific project
  configure_file(Doxyfile.in 
                 ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)
  #-- Add a custom target to run Doxygen when ever the project is built
  add_custom_target (doc   
                     COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                     SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
  # IF you want the documentation to be generated EVERY time you build the project
  # then add the 'ALL' keyword after "doc" from the above command.
endif()

########################################################################
#
# Install
#
########################################################################

install(DIRECTORY src/vli DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.ipp" PATTERN ".svn" EXCLUDE)
install(FILES ${PROJECT_BINARY_DIR}/src/vli/libvli.a DESTINATION lib)
install(FILES ${PROJECT_BINARY_DIR}/vli/config.hpp DESTINATION include/vli/)
if(VLI_COMPILE_GPU)
install(FILES ${PROJECT_BINARY_DIR}/src/vli/libaccelerator_gpu.a DESTINATION lib)
endif(VLI_COMPILE_GPU)
