cmake_minimum_required (VERSION 2.8)
project (castle_squares)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/ext/cmake)

set(CASTLE_INCLUDE_DIR 	${CMAKE_CURRENT_LIST_DIR}/include)
set(CASTLE_SOURCE_DIR 	${CMAKE_CURRENT_LIST_DIR}/src)
set(CASTLE_BUILD_DIR 	${CMAKE_CURRENT_LIST_DIR}/build)
set(CASTLE_LIB_DIR 		${CMAKE_CURRENT_LIST_DIR}/lib)
set(CASTLE_SHADERS_DIR 	${CMAKE_CURRENT_LIST_DIR}/shaders)
set(GLFW_ROOT_DIR 		${CASTLE_LIB_DIR}/glfw-3.1.1)
set(GLEW_ROOT_DIR 		${CASTLE_LIB_DIR}/glew-1.12.0)


file(GLOB CASTEL_SOURCES ${CASTLE_SOURCE_DIR}/*.cpp)
file(GLOB CASTEL_INCLUDES ${CASTLE_INCLUDE_DIR}/*.hpp)
add_executable(castle_squares ${CASTEL_SOURCES} ${CASTEL_INCLUDES})

#castle_squares
include_directories(
	${CASTLE_INCLUDE_DIR}
	${CASTLE_SOURCE_DIR}
) 

#ogl
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
  MESSAGE(ERROR "OpenGL environment missing")
endif (OPENGL_FOUND)


#glfw
add_subdirectory(${GLFW_ROOT_DIR})
include_directories(${GLFW_ROOT_DIR}/include)
target_link_libraries(castle_squares glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})


#glm - no need to build
include_directories(lib/glm)


#glew
find_package(GLEW)
if(GLEW_FOUND)
	message("GLEW FOUND")
	include_directories(${GLEW_INCLUDE_DIR})
	target_link_libraries(castle_squares glew ${GLEW_LIBRARIES})
else()
	#make this global, so we can access it from subproject
	set(GLEW_BUILD_INFO OFF CACHE BOOL "")
	add_subdirectory(${GLEW_ROOT_DIR}) 
	include_directories(${GLEW_ROOT_DIR}/include)
	target_link_libraries(castle_squares glew32 ${GLEW_LIBRARIES})
	if (WIN32)
	  set(GLEW_LIBRARY ${GLEW_LIBRARY})
	else()
	  set(GLEW_LIBRARY ${GLEW_LIBRARY}s)
	endif()
endif()

#shaders
file(COPY ${CASTLE_SHADERS_DIR} DESTINATION ${CASTLE_BUILD_DIR})