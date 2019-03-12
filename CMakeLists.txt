cmake_minimum_required (VERSION 3.12)

project("tw")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/lib/sdl2-cmake-scripts")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")

configure_file (
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)

include_directories("${PROJECT_BINARY_DIR}")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(tw src/main.c)

install(TARGETS tw RUNTIME DESTINATION bin)
install(DIRECTORY resources/share/ DESTINATION share)

target_include_directories(tw PRIVATE lib/uthash/include)

add_subdirectory(src)

target_link_libraries(tw
	m
	lua
	SDL2_image
	SDL2
	SDL2_ttf
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_link_libraries(tw
		"-framework OpenGL"
	)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(tw
		OpenGL
	)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
