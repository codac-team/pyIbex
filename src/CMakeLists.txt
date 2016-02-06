configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pyIbex_Setting.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/pyIbex_Setting.h
)

set( pip_dir "${CMAKE_CURRENT_BINARY_DIR}/pip_package")
include_directories(.)
add_subdirectory(core)
#add_subdirectory(predicate)
#add_subdirectory(wip)


install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pyIbex" DESTINATION "${PYTHON_INSTDIR}")


configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/pip_package/setup.py
)

add_custom_target(pip_package)
add_custom_command(TARGET pip_package PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/pyIbex/" "${CMAKE_CURRENT_BINARY_DIR}/pip_package/pyIbex"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_core> "${pip_dir}/pyIbex/core"
    # COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:wip> "${pip_dir}/pyIbex/wip"
    COMMAND ${PYTHON_EXECUTABLE} ARGS setup.py bdist_wheel
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pip_package
)

add_dependencies(pip_package pyIbex)
# add_dependencies(pip_package wip)




