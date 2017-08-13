
set( SRCS
      pyibex_SepProj.cpp
      pyibex_SepFixPoint.cpp
      pyibex_SepCtcPairProj.cpp
      pyibex_SepTransform.cpp
      pyibex_CtcTransform.cpp
      pyibex_QInterProjF.cpp

		)
set( HEADERS
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepProj.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepFixPoint.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepCtcPairProj.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepTransform.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_CtcTransform.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_QInterProjF.h

		)

# add_subdirectory(polar)
# FILE(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

add_library(pyibexlib ${SRCS} ${HEADERS})
target_link_libraries(pyibexlib ${LIBS})

# INSTALL( TARGETS pyibexlib DESTINATION lib)
# install(FILES ${HEADERS} DESTINATION "include/ibex-geometry")


# PYBIND11_ADD_MODULE(pyibex-geometry SHARED
    # pyIbex_geometry.cpp
#
# )
# target_link_libraries(pyibex-geometry ${LIBS} ibex-geometry)

# add_custom_command(TARGET pyibex-geometry POST_BUILD
	# COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:pyibex-geometry>" "${PYTHON_PACKAGE_DIR}/pyibex-geometry"
# )
