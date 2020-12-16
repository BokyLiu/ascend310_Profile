cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11 -fstack-protector-all -Wl,-z,relro,-z,now,-z,noexecstack)
add_compile_options(-g)
project(ascend310_Profile)

set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(PROJECT_SRC_ROOT ${CMAKE_CURRENT_LIST_DIR}/../)
set(CMAKE_MODULE_PATH ${PROJECT_SRC_ROOT}/build/CMake/)

# ddk
find_package(DDK REQUIRED)
# opencv 
find_package(OpenCV REQUIRED)

################################### set compile and target ##############################
add_subdirectory(host)
add_subdirectory(device)

################################### other operate ##############################
# copy graph.config file
file(COPY ${PROJECT_SRC_ROOT}/graph.config
        DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY ${PROJECT_SRC_ROOT}/compare.py
#       DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY ${PROJECT_SRC_ROOT}/show_modelOut.py
#        DESTINATION ${CMAKE_BINARY_DIR})
