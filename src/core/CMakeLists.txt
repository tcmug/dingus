
add_subdirectory(ui)
add_subdirectory(math)
add_subdirectory(resources)

file(GLOB sources
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

target_sources(tw PRIVATE
	${sources}		
)