# PYBIND11_ADD_MODULE(pytube SHARED  pyIbex_Tube.cpp )
# target_link_libraries(pytube  ibex-robotics-tube ibex-robotics-graphics)
# set_target_properties(pytube PROPERTIES OUTPUT_NAME tube)
# add_custom_command(TARGET pytube POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:pypolar>" "${python_package_dir}"
# )
# set_target_properties(tube PROPERTIES OUTPUT_NAME polar)

PYBIND11_ADD_MODULE(pypolar SHARED  pyIbex_Polar.cpp )
# target_link_libraries(pypolar ${LIBS} ibex-robotics-polar)
target_link_libraries(pypolar ${LIBS} ibex-robotics-polar)
set_target_properties(pypolar PROPERTIES OUTPUT_NAME polar)
add_custom_command(TARGET pypolar POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:pypolar>" "${PYTHON_PACKAGE_DIR}/pyIbex"
)
