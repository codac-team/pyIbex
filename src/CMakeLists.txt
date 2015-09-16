configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pyIbex_Setting.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/pyIbex_Setting.h
)

#add_subdirectory(ibex)
INCLUDE_DIRECTORIES("core")
#INCLUDE_DIRECTORIES("ibex/alg")
#INCLUDE_DIRECTORIES("ibex/geometry")
#INCLUDE_DIRECTORIES("ibex/set")
file(GLOB CORE_SRCS "core/pyIbex_*.cpp")

# set(CORE_SRCS ${CORE_SRCS} "core/ibex_SepProj.cpp" "core/ibex_SepCtcPairProj.cpp" "core/ibex_SepFixPoint.cpp")

PYTHON_ADD_MODULE(pyIbex pyIbex.cpp ${CORE_SRCS} "core/pyIbex_to_python_converter.h" ${IBEX_ADDON_SRCS})
target_link_libraries(pyIbex ${LIBS})
# set_target_properties(pyIbex PROPERTIES SUFFIX ".pyd")
install( TARGETS pyIbex DESTINATION "${PYTHON_INSTDIR}/pyIbex")
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pyIbex" DESTINATION "${PYTHON_INSTDIR}")

#install( FILE "pySIVIA.py" DESTINATION ${PYTHON_INSTDIR}/pyIbex)


