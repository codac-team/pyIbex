cmake_minimum_required(VERSION 2.8.3)

# build type
IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE "DEBUG")
	#SET(CMAKE_BUILD_TYPE "RELEASE")
	#SET(CMAKE_BUILD_TYPE "RELWITHDEBINFO")
	#SET(CMAKE_BUILD_TYPE "MINSIZEREL")
ENDIF()

# option declaration
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLE "Build example" ON)

# option(USE_PYTHON3 OFF)



FIND_PACKAGE(PythonInterp 2.7 REQUIRED)
FIND_PACKAGE(PythonLibs 2.7 REQUIRED)
IF(PYTHONLIBS_FOUND)
	INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIRS}")
ELSE()
	MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()

FIND_PACKAGE(Boost COMPONENTS python)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}")
  SET(Boost_USE_STATIC_LIBS     OFF)
  SET(Boost_USE_MULTITHREADED    ON)
  SET(Boost_USE_STATIC_RUNTIME     OFF)
  FIND_PACKAGE(Boost  ${BOOST_VERSION} COMPONENTS python) 
  SET(LIBS ${LIBS} ${Boost_LIBRARIES})
ELSEIF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find Boost.")
ENDIF()

option(WITH_VIBES "Include Vibes output" ON)
if(WITH_VIBES)
	set(_PYIBEX_WITH_VIBES_ 1)
endif(WITH_VIBES)


# Find Ibex libs
find_package(PkgConfig)
pkg_check_modules(PC_IBEX REQUIRED ibex)
add_definitions(${PC_IBEX_CFLAGS_OTHER})
include_directories(${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${PC_IBEX_INCLUDES})
set(LIBS ${LIBS} -L${PC_IBEX_LIBDIR} ${PC_IBEX_LIB_DIRS} ${PC_IBEX_LIBRARIES})
message(STATUS ${LIBS})
set(PACKAGE_VERSION "0.9.0")

# Find python install path (from morse install script)
EXECUTE_PROCESS(COMMAND
					${PYTHON_EXECUTABLE} -c "import distutils.sysconfig, sys; sys.stdout.write(distutils.sysconfig.get_python_lib(1,0,\"\"))"
					OUTPUT_VARIABLE PYTHON_INSTALL
					ERROR_VARIABLE PYTHON_STDERR
					RESULT_VARIABLE PYTHON_ERR
				)
if(NOT PYTHON_INSTDIR)
	SET(PYTHON_INSTDIR ${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL}) 
endif(NOT PYTHON_INSTDIR)
if(WIN32)
    STRING(REGEX REPLACE "\\\\" "/" PYTHON_INSTDIR ${PYTHON_INSTDIR})
endif(WIN32)
MESSAGE(STATUS "will install python files in ${PYTHON_INSTDIR}")

add_subdirectory(src)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC"  )

if (BUILD_TESTS)
	ENABLE_TESTING()
	include( CTest )
	MESSAGE(STATUS ${CTEST_ENVIRONMENT})
	add_subdirectory(tests)
endif(BUILD_TESTS)

if (BUILD_EXAMPLE)
	add_subdirectory(examples)
endif(BUILD_EXAMPLE)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Benoit Desrochers") #required
 
INCLUDE(CPack)
