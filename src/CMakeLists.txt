configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pyIbex_Setting.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/pyIbex_Setting.h
)


INCLUDE_DIRECTORIES("core")
file(GLOB CORE_SRCS "core/pyIbex_*.cpp")
INCLUDE_DIRECTORIES("Paving")
set( CORE_SRCS ${CORE_SRCS} ./Paving/paving.cpp ./Paving/pyIbex_Paving.cpp)
#add_subdirectory(Paving)
#set(LIBS ${LIBS} Paving)

PYTHON_ADD_MODULE(pyIbex pyIbex.cpp ${CORE_SRCS} )

target_link_libraries(pyIbex ${LIBS})

install( TARGETS pyIbex DESTINATION "${PYTHON_INSTDIR}/pyIbex")
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pyIbex" DESTINATION "${PYTHON_INSTDIR}")




