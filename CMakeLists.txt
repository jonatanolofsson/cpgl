cmake_minimum_required(VERSION 2.8)
project(CPGL)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -ggdb -Wall -Wextra -fPIC")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic -ldl")

# add our macro directory to cmake search path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(YAMLCPP_STATIC_LIBRARY ON)

find_package(GLUT)
find_package(OpenGL)
find_package(YamlCpp REQUIRED)
find_package( Eigen3 )
find_package( Boost COMPONENTS thread )
find_package(OpenCL)

add_subdirectory(lib)
include_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/elements
    ${PROJECT_SOURCE_DIR}/include
    ${YAMLCPP_INCLUDE_DIR}
    ${Eigen3_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OPENCL_INCLUDE_DIR}
)

link_directories ( ${Boost_LIBRARY_DIRS} )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -ggdb -Wall -Wextra -fPIC")

add_definitions(-DGL_GLEXT_PROTOTYPES)

option(CPGL_TEST ON)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_BINARY_DIR})

set(SRC_DIR "src")
add_library(CPGL SHARED
    ${SRC_DIR}/cpgl.cpp
    ${SRC_DIR}/glut.cpp
    ${SRC_DIR}/tools.cpp
    ${SRC_DIR}/window.cpp
    ${SRC_DIR}/baseelement.cpp
    ${SRC_DIR}/opencl.cpp
    )
target_link_libraries(CPGL
    ${Boost_LIBRARIES}
    glut
    GL
    GL_tools
    ${YAMLCPP_LIBRARY}
    ${OPENCL_LIBRARIES}
)

if(CPGL_TEST)
    add_executable(test main.cpp)
    target_link_libraries(test
        ${Boost_LIBRARIES}
        CPGL
        ${YAMLCPP_LIBRARY}
    )
    add_subdirectory(elements)
endif(CPGL_TEST)





## Installation

set(header_directory "include/")
set(_library_dir lib)

set(INCLUDE_INSTALL_ROOT_DIR include)

set(INCLUDE_INSTALL_DIR ${INCLUDE_INSTALL_ROOT_DIR}/cpgl)
set(LIB_INSTALL_DIR "${_library_dir}${LIB_SUFFIX}")

set(_INSTALL_DESTINATIONS
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION "lib${LIB_SUFFIX}"
)

install(TARGETS CPGL ${_INSTALL_DESTINATIONS})
install(
    DIRECTORY ${header_directory}
    DESTINATION ${INCLUDE_INSTALL_DIR}
    FILES_MATCHING PATTERN "*.h*"
)
