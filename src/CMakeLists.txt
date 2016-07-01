configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pyIbex_Setting.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/pyIbex_Setting.h
)



# Installation process handled by setup.py
set(PYTHON_PACKAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/python_package")
file(MAKE_DIRECTORY ${PYTHON_PACKAGE_DIR})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/pyIbex/" "${PYTHON_PACKAGE_DIR}/pyIbex")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
  ${PYTHON_PACKAGE_DIR}/setup.py
)

set( pip_dir "${CMAKE_CURRENT_BINARY_DIR}/pip_package")
include_directories(.)
add_subdirectory(core)
add_subdirectory(robotics)


install( DIRECTORY "${PYTHON_PACKAGE_DIR}/pyIbex" DESTINATION "${PYTHON_INSTDIR}")

add_custom_target(pip_package)
add_custom_command(TARGET pip_package PRE_BUILD
    COMMAND ${PYTHON_EXECUTABLE} ARGS setup.py bdist_wheel -d ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${PYTHON_PACKAGE_DIR}
)

# add_dependencies(pip_package pyIbex)
# add_dependencies(pip_package wip)
