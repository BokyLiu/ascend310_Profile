if(NOT OPENCV_FOUND)
	# find_path(OPENCV_INCLUDE_DIRS opencv2/opencv.hpp /home/HwHiAiUser/wepleo/project/opencv/include)
	set(OPENCV_INCLUDE_DIRS /home/HwHiAiUser/wepleo/project/opencv/include)
	
	set(OPENCV_LIB_PATH 
		/home/HwHiAiUser/wepleo/project/opencv/lib/)

	find_library(OPENCV_CORE NAMES opencv_core PATHS	${OPENCV_LIB_PATH} NO_CMAKE_SYSTEM_PATH DOC "The OpenCV core library")
	find_library(OPENCV_HIGHGUI NAMES opencv_highgui PATHS	${OPENCV_LIB_PATH} NO_CMAKE_SYSTEM_PATH DOC "The OpenCV highgui library")
	find_library(OPENCV_IMGPROC NAMES opencv_imgproc PATHS	${OPENCV_LIB_PATH} NO_CMAKE_SYSTEM_PATH DOC "The OpenCV imgproc library")
	find_library(OPENCV_IMGCODECS NAMES opencv_imgcodecs PATHS	${OPENCV_LIB_PATH} NO_CMAKE_SYSTEM_PATH DOC "The OpenCV imgproc library")

	find_library(OPENCV_VIDEO NAMES opencv_video PATHS	${OPENCV_LIB_PATH} NO_CMAKE_SYSTEM_PATH DOC "The OpenCV video library")
	find_library(OPENCV_DNN NAMES opencv_dnn PATHS	${OPENCV_LIB_PATH} NO_CMAKE_SYSTEM_PATH DOC "The OpenCV video library")

	set(OpenCV_LIBS ${OPENCV_CORE}  ${OPENCV_HIGHGUI}  ${OPENCV_VIDEO} ${OPENCV_IMGPROC} ${OPENCV_IMGCODECS} ${OPENCV_DNN})

	# file(GLOB OpenCV_LIBS "/home/HwHiAiUser/wepleo/project/opencv/lib/libopencv*")


	message("==========OpenCV_LIBS:${OpenCV_LIBS}=============")
	if(OPENCV_INCLUDE_DIRS)
		set(OPENCV_FOUND 1)
	else(OPENCV_INCLUDE_PATH)
		set(OPENCV_FOUND 0)
		message(FATAL_ERROR "OpenCV not found.  Either set OPENCV_INCLUDE_PATH and OPENCV_LIBRARIES or make sure OpenCV is included in your PATH")
	endif(OPENCV_INCLUDE_DIRS)
    
	set(OPENCV_FOUND TRUE)
endif(NOT OPENCV_FOUND)
