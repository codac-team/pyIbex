cmake_minimum_required(VERSION 2.8.3)


# build type
IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE "RELEASE")
	#SET(CMAKE_BUILD_TYPE "DEBUG")
	#SET(CMAKE_BUILD_TYPE "RELWITHDEBINFO")
	#SET(CMAKE_BUILD_TYPE "MINSIZEREL")
ENDIF()

# option declaration
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLE "Build example" OFF)
option(BUILD_STATIC "make standalone lib" ON)
option(USE_PYTHON3 ON "use python3")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/check_dependancies.cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC"  )
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

if (BUILD_TESTS)
	ENABLE_TESTING()
	include( CTest )
	add_subdirectory(tests)
endif(BUILD_TESTS)

if (BUILD_EXAMPLE)
	add_subdirectory(examples)
endif(BUILD_EXAMPLE)


#########################################################################
### Uninstall ###
#########################################################################
configure_file("${CMAKE_HOME_DIRECTORY}/CMakeModules/cmake_uninstall.cmake.in" "cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake_uninstall.cmake")


SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Benoit Desrochers") #required
 INCLUDE(CPack)
