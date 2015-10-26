configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pyIbex_Setting.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/pyIbex_Setting.h
)


INCLUDE_DIRECTORIES("core")

file(GLOB CORE_SRCS "core/pyIbex_*.cpp")

add_subdirectory(wip)
PYTHON_ADD_MODULE(pyIbex pyIbex.cpp ${CORE_SRCS} wip/pyIbex_wip.cpp )
target_link_libraries(pyIbex ${LIBS} wip)


install( TARGETS pyIbex DESTINATION "${PYTHON_INSTDIR}/pyIbex")
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pyIbex" DESTINATION "${PYTHON_INSTDIR}")


configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/pip_package/setup.py
 )


add_custom_target(pip_package)
add_custom_command( TARGET pip_package PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/pyIbex" "${CMAKE_CURRENT_BINARY_DIR}/pip_package/pyIbex"
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pyIbex> "${CMAKE_CURRENT_BINARY_DIR}/pip_package/pyIbex/"
)
add_custom_command(TARGET pip_package POST_BUILD
	  COMMAND ${PYTHON_EXECUTABLE} ARGS setup.py bdist_wheel
	  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pip_package
)

add_dependencies(pip_package pyIbex)




