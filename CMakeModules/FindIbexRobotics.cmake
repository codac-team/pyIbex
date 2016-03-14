# - Try to find IBEX
# Once done this will define
#  IBEX_FOUND - System has ibex
#  IBEX_INCLUDE_DIRS - The ibex include directories
#  IBEX_LIBRARIES - The libraries needed to use ibex
#  IBEX_DEFINITIONS - Compiler switches required for using ibex

find_package(PkgConfig)
pkg_check_modules(PC_IBEX_ROBOTICS QUIET ibexRobotics)
message(STATUS "IBEX_ROBOTICS_ROOT ${IBEX_ROBOTICS_ROOT}")

if(IBEX_USE_STATIC)
  SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif()

#set(IBEX_DEFINITIONS ${PC_IBEX_CFLAGS_OTHER})
find_path(POLAR_INCLUDE_DIR ibex_CtcPolar.h
          HINTS ${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${IBEX_ROBOTICS_ROOT}
				PATH_SUFFIXES include include/ibex-robotics )

find_library(POLAR_LIBRARY NAMES ibex-robotics-polar
            HINTS ${PC_IBEX_LIBDIR} ${PC_IBEX_LIBRARY_DIRS}  ${IBEX_ROBOTICS_ROOT}
            PATH_SUFFIXES lib
						)

set(IBEX_ROBOTICS_LIBRARIES  ${POLAR_LIBRARY})
set(IBEX_ROBOTICS_INCLUDE_DIRS ${POLAR_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set IBEX_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(IBEX_ROBOTICS  DEFAULT_MSG
                                  IBEX_ROBOTICS_LIBRARIES IBEX_ROBOTICS_INCLUDE_DIRS)

mark_as_advanced(IBEX_ROBOTICS_INCLUDE_DIRS IBEX_ROBOTICS_LIBRARIES )
