
file(GLOB sources
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

target_sources(tw PRIVATE
	${sources}		
)


