project(host)

add_definitions(-DATLAS_HOST)

####################################### config the host compile #############################
set(HOST_TARGET ascend310_Profile)
set(HOST_INCUDUE_DIR
    ${DDK_INCLUDE_DIRS}
    ${DDK_CEREAL_INCLUDE_DIRS}
    ${DDK_PROTOBUF_INCLUDE_DIRS}
    ${OPENCV_INCLUDE_DIRS}
    ${PROJECT_SRC_ROOT}/ 
    ${PROJECT_SRC_ROOT}/HostPreProcessEngine/ 
    ${PROJECT_SRC_ROOT}/HostPostProcessEngine/ 
    ${PROJECT_SRC_ROOT}/common)

file(GLOB SRC ${PROJECT_SRC_ROOT}/HostPreProcessEngine/*.cpp ${PROJECT_SRC_ROOT}/*.cpp ${PROJECT_SRC_ROOT}/HostPostProcessEngine/*.cpp)
# file(GLOB OPENCV_LIB_2 "/home/HwHiAiUser/wepleo/project/opencv/lib/libopencv*")
set(HOST_SOURCE_FILE
    ${SRC})

set(HOST_SHARED_LIB_DIR
    )

message("2==========OPENCV_INCLUDE_DIRS:${OPENCV_INCLUDE_DIRS}=============")

message("2==========OpenCV_LIBS:${OpenCV_LIBS}=============")

set(HOST_SHARED_LIB 
    ${DDK_HOST_LIBRARIES}  
    ${DDK_PROTOBUF_LIBRARYS} 
    # ${OPENCV_LIB_2}
    ${OpenCV_LIBS}
    pthread )

####################################### host compile #############################

link_directories(${HOST_SHARED_LIB_DIR})
add_executable(${HOST_TARGET} ${HOST_SOURCE_FILE})
target_include_directories(${HOST_TARGET} PUBLIC ${HOST_INCUDUE_DIR})
target_link_libraries(${HOST_TARGET} ${HOST_SHARED_LIB})