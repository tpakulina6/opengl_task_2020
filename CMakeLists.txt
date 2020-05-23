cmake_minimum_required(VERSION 3.0)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# CUDA
# Promt: for finding CUDA you can set -DCUDA_TOOLKIT_ROOT_DIR
find_package(CUDA)
if (CUDA_FOUND)
    message("Cuda ${CUDA_VERSION} is found under ${CUDA_TOOLKIT_ROOT_DIR}")

    # Define architectures we compile for
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} --std c++14
            -gencode=arch=compute_30,code=sm_30
            -gencode=arch=compute_35,code=sm_35
            -gencode=arch=compute_50,code=sm_50
            -gencode=arch=compute_52,code=sm_52
            --default-stream per-thread)
    if(CUDA_VERSION GREATER_EQUAL 8)
        set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}
                -gencode=arch=compute_60,code=sm_60
                -gencode=arch=compute_61,code=sm_61
                -gencode=arch=compute_61,code=compute_61)
    endif()

    set(CUDA_NVCC_FLAGS_DEBUG "${CUDA_NVCC_FLAGS_DEBUG} -lineinfo")
    set(CUDA_NVCC_FLAGS_RELEASE "${CUDA_NVCC_FLAGS_RELEASE} -O2 -D_FORCE_INLINES")
endif()

# We are using shared libs.
set(BUILD_SHARED_LIBS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake Targets")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# OpenGL core profile definition
option(USE_CORE_PROFILE "Enable to use core profile" OFF)

include(MacroUtils)

set(SELECTED_STUDENT CACHE STRING "The only student to build")

add_definitions(-DGLM_FORCE_PURE)
add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-DGLM_FORCE_DEPTH_ZERO_TO_ONE)
add_subdirectory(external)


ADD_TASKS(task1)
ADD_TASKS(task2)
ADD_TASKS(task3)
