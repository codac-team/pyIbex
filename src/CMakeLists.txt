
file(GLOB SRCS "pyIbex_*.cpp")
PYTHON_ADD_MODULE(pyIbex pyIbex.cpp ${SRCS})
target_link_libraries(pyIbex ${LIBS})

install( TARGETS pyIbex DESTINATION ${PYTHON_INSTDIR})



