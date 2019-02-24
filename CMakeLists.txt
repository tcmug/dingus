cmake_minimum_required (VERSION 3.12)



project (Dingus)

# -ggdb
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/lib/sdl2-cmake-scripts")

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

target_include_directories(dingus PRIVATE lib/uthash/include)

target_sources(dingus
	PRIVATE
		src/components/text.c
		src/components/right.c
		src/components/top.c
		src/components/center.c
		src/components/view.c		
		src/core/engine.c
		src/core/buffer.c
		src/core/print.c
		src/core/shader.c		
		src/core/component.c
		src/core/texture.c
		src/math/vector.c
		src/math/matrix.c
		src/math/sphere.c
		src/math/triangle.c
		src/math/rectangle.c
		src/math/nettle.c
# 		src/fun/body/sphere.c
#		src/fun/body/triangle.c
)

target_link_libraries(dingus
	m
	SDL2_image
	SDL2
	SDL2_ttf
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_link_libraries(dingus
		"-framework OpenGL"
	)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(dingus
		OpenGL
	)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
