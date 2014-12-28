cmake_minimum_required(VERSION 2.8.3)

option(BUILD_TESTS "Build tests" ON)

FIND_PACKAGE(PythonInterp)
FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(Boost COMPONENTS python)

ENABLE_TESTING()
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES}) # Deprecated but so convenient!


find_package(PkgConfig)
pkg_check_modules(PC_IBEX REQUIRED ibex)
add_definitions(${PC_IBEX_CFLAGS_OTHER})
include_directories(${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${PC_IBEX_INCLUDES})
set(LIBS ${LIBS} -L/home/biop/programmation/devel/lib ${PC_IBEX_LIB_DIRS} ${PC_IBEX_LIBRARIES})

set(PACKAGE_VERSION "0.0.1")

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

#add_executable(titi test.cpp)
#target_link_libraries(titi ${LIBS})
add_subdirectory(src)

if (BUILD_TESTS)
	include( CTest )
	MESSAGE(STATUS ${CTEST_ENVIRONMENT})
	add_subdirectory(tests)
endif(BUILD_TESTS)


#FILE(COPY ./tests/test_Interval.py DESTINATION .)
#FILE(COPY ./tests/test_IntervalVector.py DESTINATION .)
#FILE(COPY ./tests/test_Bsc.py DESTINATION .)
#FILE(COPY ./tests/test_Function.py DESTINATION .)
#FILE(COPY ./tests/test_SIVIA.py DESTINATION .)
#ADD_TEST(pyIbex_Interval test_Interval.py)
#ADD_TEST(pyIbex_IntervalVector test_IntervalVector.py)
#ADD_TEST(pyIbex_Bsc test_Bsc.py)
#ADD_TEST(pyIbex_Function test_Function.py)
