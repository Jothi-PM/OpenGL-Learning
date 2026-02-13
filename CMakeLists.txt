cmake_minimum_required(VERSION 3.26)
project(OpenGL-Learning)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glfw/include)
set(GLFW_LIB_DIR     ${CMAKE_SOURCE_DIR}/external/glfw/lib)

set(GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glew/include)
set(GLEW_LIB_DIR     ${CMAKE_SOURCE_DIR}/external/glew/lib)

set(GLM_INCLUDE_DIR  ${CMAKE_SOURCE_DIR}/external/glm)

function(copy_runtime_libs target dll_dir dll_name)
add_custom_command(TARGET ${target} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${dll_dir}/${dll_name}.dll"
		$<TARGET_FILE_DIR:${target}>
)
endfunction()
add_subdirectory(05_MiniCamera_app)
