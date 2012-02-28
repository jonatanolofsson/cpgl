set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c99")
add_library(GL_tools SHARED GL_utilities.c loadobj.c LoadTGA.c)
