set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/elements)

# Modules with default configuration
set(SUBDIRS
    camera
    ground
    skybox
    flyer
)

foreach(dir ${SUBDIRS})
add_subdirectory(${dir})
endforeach(dir)
