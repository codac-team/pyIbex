
set( SRCS
      pyibex_SepProj.cpp
      pyibex_SepFixPoint.cpp
      pyibex_SepCtcPairProj.cpp
      pyibex_SepTransform.cpp
      pyibex_CtcTransform.cpp
      pyibex_QInterProjF.cpp
      pyibex_SepUnionBbox.cpp
      pyibex_CtcHull.cpp
      # vibes.cpp
		)
set( HEADERS
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepProj.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepFixPoint.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepCtcPairProj.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepUnionBbox.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepTransform.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_CtcTransform.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_QInterProjF.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_CtcHull.h
      # ${CMAKE_CURRENT_SOURCE_DIR}/vibes.h

		)

# add_subdirectory(polar)
# FILE(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

add_library(pyibexlib ${SRCS} ${HEADERS})
target_link_libraries(pyibexlib ${LIBS})
if(NOT MSVC)
  # Make sure C++11/14 are enabled
  target_compile_options(pyibexlib PUBLIC ${PYBIND11_CPP_STANDARD})
endif()

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
