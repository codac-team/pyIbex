cmake_minimum_required(VERSION 2.8.3)

# option declaration
option(BUILD_TESTS "Build tests" ON)

FIND_PACKAGE(PythonInterp)
FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(Boost COMPONENTS python)


#option(USE_OPENCV OFF)
#option(USE_GDAL OFF)
#option(USE_PYTHON3 OFF)
option(WITH_VIBES "Include Vibes output" OFF)
if(WITH_VIBES)
	set(_PYIBEX_WITH_VIBES_ 1)
endif(WITH_VIBES)


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES}) # Deprecated but so convenient!

# Find Ibex libs
find_package(PkgConfig)
pkg_check_modules(PC_IBEX REQUIRED ibex)
add_definitions(${PC_IBEX_CFLAGS_OTHER})
include_directories(${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${PC_IBEX_INCLUDES})
set(LIBS ${LIBS} -L${PC_IBEX_LIBDIR} ${PC_IBEX_LIB_DIRS} ${PC_IBEX_LIBRARIES})
message(STATUS ${LIBS})
set(PACKAGE_VERSION "0.9.0")

# Find python install path
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

if (BUILD_TESTS)
	ENABLE_TESTING()
	include( CTest )
	MESSAGE(STATUS ${CTEST_ENVIRONMENT})
	add_subdirectory(tests)
endif(BUILD_TESTS)
