cmake_minimum_required (VERSION 3.12)

set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)
 
project (Dingus)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

configure_file (
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)

include_directories("${PROJECT_BINARY_DIR}")
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(dingus src/main.c)

install(TARGETS dingus RUNTIME DESTINATION bin)

install(DIRECTORY resources/share/ DESTINATION share)

target_sources(dingus
	PRIVATE
		src/components/text.c
		src/components/right.c
		src/components/center.c
		src/core/engine.c
		src/core/print.c
		src/core/component.c
)

target_link_libraries(dingus
	m
	OpenGL
	SDL2
	SDL2_ttf
)
