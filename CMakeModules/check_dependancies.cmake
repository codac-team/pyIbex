#check_dependancy.cmake

if(USE_PYTHON3)
	set(PYTHON_MIN_VERSION 3.4)
  set(PythonInterp_FIND_VERSION "${PYTHON_MIN_VERSION}")
  set(PythonLibs_FIND_VERSION "${PYTHON_MIN_VERSION}")
  find_package(PythonInterp REQUIRED)
  find_package(PythonLibs REQUIRED)

else (USE_PYTHON3)
	set(PYTHON_MIN_VERSION 2.7)
  set(PythonInterp_FIND_VERSION "${PYTHON_MIN_VERSION}")
  set(PythonLibs_FIND_VERSION "${PYTHON_MIN_VERSION}")
  find_package(PythonInterp REQUIRED)
  find_package(PythonLibs REQUIRED)
endif(USE_PYTHON3)


find_package(PkgConfig REQUIRED)


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
  SET(Boost_USE_STATIC_RUNTIME     ON)
  FIND_PACKAGE(Boost  ${BOOST_VERSION} COMPONENTS python) 
  SET(LIBS ${LIBS} ${Boost_LIBRARIES})
  #if(BUILD_STATIC)
  #	add_definitions(-DBOOST_PYTHON_STATIC_LIB)
  #endif(BUILD_STATIC)

ELSEIF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find Boost.")
ENDIF()

#option(WITH_VIBES "Include Vibes output" ON)
#if(WITH_VIBES)
#	set(_PYIBEX_WITH_VIBES_ 1)
#endif(WITH_VIBES)


# Find Ibex libs
find_package(PkgConfig)
pkg_check_modules(PC_IBEX REQUIRED ibex)
add_definitions(${PC_IBEX_CFLAGS_OTHER})
include_directories(${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${PC_IBEX_INCLUDES})
set(LIBS ${LIBS} -L${PC_IBEX_LIBDIR} ${PC_IBEX_LIB_DIRS} ${PC_IBEX_LIBRARIES})
message(STATUS ${LIBS})