# Locate cpplot
#
# This module defines
#  CPPLOT_FOUND, if false, do not try to link to cpplot
#  CPPLOT_LIBRARY, where to find cpplot
#  CPPLOT_INCLUDE_DIR, where to find cpplot.hpp
#
# By default, the dynamic libraries of cpplot will be found. To find the static ones instead,
# you must set the CPPLOT_STATIC_LIBRARY variable to TRUE before calling find_package(cpplot ...).
#
# If cpplot is not installed in a standard path, you can use the CPPLOT_DIR CMake variable
# to tell CMake where cpplot is.

# attempt to find static library first if this is set
if(CPPLOT_STATIC_LIBRARY)
    set(CPPLOT_STATIC libcpplot.a)
endif()

# find the cpplot include directory
find_path(CPPLOT_INCLUDE_DIR cpplot/cpplot.hpp
          PATH_SUFFIXES include
          PATHS
          ~/Library/Frameworks/cpplot/include/
          /Library/Frameworks/cpplot/include/
          /usr/local/include/
          /usr/include/
          /opt/local/cpplot/
          /opt/csw/cpplot/    # Blastwave
          /opt/cpplot/
          ${CPPLOT_DIR}/include/)

# find the cpplot library
find_library(CPPLOT_LIBRARY
             NAMES ${CPPLOT_STATIC} cpplot
             PATH_SUFFIXES lib64 lib
             PATHS ~/Library/Frameworks
                    /Library/Frameworks
                    /usr/local
                    /usr
                    /sw
                    /opt/local
                    /opt/csw
                    /opt
                    ${CPPLOT_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set CPPLOT_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CPPLOT DEFAULT_MSG CPPLOT_INCLUDE_DIR CPPLOT_LIBRARY)
mark_as_advanced(CPPLOT_INCLUDE_DIR CPPLOT_LIBRARY)
