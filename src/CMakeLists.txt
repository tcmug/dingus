
add_subdirectory(core)
add_subdirectory(components)

target_sources(tw PRIVATE	
	src/fun/body/sphere.c
	src/fun/body/triangle.c
)
