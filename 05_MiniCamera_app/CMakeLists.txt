add_executable(camera camera.cpp Texture.cpp shaders.h)

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
find_package(OpenCV REQUIRED)

target_include_directories(camera PRIVATE 
				${GLFW_INCLUDE_DIR}
				${GLEW_INCLUDE_DIR}
				${GLM_INCLUDE_DIR}
				${PROJECT_SOURCE_DIR}
                ${OpenCV_INCLUDE_DIRS})

target_link_directories(camera PRIVATE 
				${GLFW_LIB_DIR}
				${GLEW_LIB_DIR}
                ${OpenCV_LIBS}) # todo:set as environment variable

target_link_libraries(camera PRIVATE
			glfw3 
			glew32 
			opengl32 # this needs to be changed for other OS
            ${OpenCV_LIBS}) 

set(CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB:MSVCRT")

copy_runtime_libs(camera ${GLFW_LIB_DIR} glfw3)
copy_runtime_libs(camera ${GLEW_LIB_DIR}/../bin glew32)
