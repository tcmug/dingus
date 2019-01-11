cmake_minimum_required (VERSION 3.12)

project (Dingus)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(dingus src/main.c)

target_sources(dingus
	PRIVATE
		src/components/text.c
		src/components/right.c
		src/components/center.c
		src/core/print.c
		src/core/component.c
)

target_link_libraries(dingus
	SDL2
	SDL2_ttf
)
