configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pyIbex_Setting.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/pyIbex_Setting.h
)


INCLUDE_DIRECTORIES("core")

file(GLOB CORE_SRCS "core/pyIbex_*.cpp")
INCLUDE_DIRECTORIES("Paving")
set( CORE_SRCS ${CORE_SRCS} ./Paving/paving.cpp ./Paving/pyIbex_Paving.cpp)

add_subdirectory(Paving)

find_package(NumPy REQUIRED)


PYTHON_ADD_MODULE(pyIbex pyIbex.cpp ${CORE_SRCS} )

target_link_libraries(pyIbex ${LIBS} ${PYTHON_LIBRARIES})


install( TARGETS pyIbex DESTINATION "${PYTHON_INSTDIR}/pyIbex")
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pyIbex" DESTINATION "${PYTHON_INSTDIR}")


configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/setup.py
 )






