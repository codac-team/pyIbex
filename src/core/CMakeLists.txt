# file(GLOB CORE_SRCS "pyIbex_*.cpp")

pybind11_add_module(pyibex SHARED
    pyIbex.cpp
    pyIbex_Interval.cpp
    pyIbex_IntervalVector.cpp
    pyIbex_Function.cpp
    pyIbex_Ctc.cpp
    pyIbex_Separators.cpp
    pyIbex_Bsc.cpp
    pyIbex_IntervalMatrix.cpp
)
target_link_libraries(pyibex PRIVATE ${LIBS})

# install( TARGETS _core DESTINATION "${PYTHON_INSTDIR}/pyIbex/core")
add_custom_command(TARGET pyibex POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:pyibex>" "${PYTHON_PACKAGE_DIR}/pyibex"
)
