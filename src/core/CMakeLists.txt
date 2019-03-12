add_subdirectory(ui)
add_subdirectory(math)

file(GLOB sources
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

target_sources(dingus
	PRIVATE
		${sources}
)