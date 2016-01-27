file(GLOB CORE_SRCS "pyIbex_*.cpp")

PYBIND11_ADD_MODULE(_core SHARED 
    pyIbex.cpp 
    pyIbex_Interval.cpp 
    pyIbex_IntervalVector.cpp  
    pyIbex_Function.cpp
    pyIbex_Ctc.cpp 
    pyIbex_Separators.cpp 
    pyIbex_Bsc.cpp 
    pyIbex_IntervalMatrix.cpp 
)
target_link_libraries(_core ${LIBS})

install( TARGETS _core DESTINATION "${PYTHON_INSTDIR}/pyIbex/core")