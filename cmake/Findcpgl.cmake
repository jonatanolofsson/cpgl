# Locate cpgl
#
# This module defines
#  CPGL_FOUND, if false, do not try to link to cpgl
#  CPGL_LIBRARY, where to find cpgl
#  CPGL_INCLUDE_DIR, where to find cpgl.hpp
#
# By default, the dynamic libraries of cpgl will be found. To find the static ones instead,
# you must set the CPGL_STATIC_LIBRARY variable to TRUE before calling find_package(cpgl ...).
#
# If cpgl is not installed in a standard path, you can use the CPGL_DIR CMake variable
# to tell CMake where cpgl is.

# attempt to find static library first if this is set
if(CPGL_STATIC_LIBRARY)
    set(CPGL_STATIC libcpgl.a)
endif()

# find the cpgl include directory
find_path(CPGL_INCLUDE_DIR cpgl/cpgl.hpp
          PATH_SUFFIXES include
          PATHS
          ~/Library/Frameworks/cpgl/include/
          /Library/Frameworks/cpgl/include/
          /usr/local/include/
          /usr/include/
          /opt/local/cpgl/
          /opt/csw/cpgl/    # Blastwave
          /opt/cpgl/
          ${CPGL_DIR}/include/)

# find the cpgl library
find_library(CPGL_LIBRARY
             NAMES ${CPGL_STATIC} CPGL
             PATH_SUFFIXES lib64 lib
             PATHS ~/Library/Frameworks
                    /Library/Frameworks
                    /usr/local
                    /usr
                    /sw
                    /opt/local
                    /opt/csw
                    /opt
                    ${CPGL_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set CPGL_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CPGL DEFAULT_MSG CPGL_INCLUDE_DIR CPGL_LIBRARY)
mark_as_advanced(CPGL_INCLUDE_DIR CPGL_LIBRARY)
