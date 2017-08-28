set( SRCS
      # Polar sources
      # pyibex_CtcAngle.cpp
      pyibex_CtcPolar.cpp
      pyibex_SepPolarXY.cpp
      # pyibex_SepPolarXYT.cpp
      # Polygon sources
      pyibex_CtcSegment.cpp
      pyibex_PdcInPolygon.cpp
      pyibex_SepPolygon.cpp
      pyibex_SepDiskExists.cpp
  )

set( HEADERS
      #Polar headers
      # ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_CtcAngle.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_CtcPolar.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepPolarXY.h
      # ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepPolarXYT.h
      #Polygon headers
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_CtcSegment.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_PdcInPolygon.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepPolygon.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepDiskExists.h
  )

# FILE(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})


add_library(ibex-geometry ${SRCS} ${HEADERS})
if(NOT MSVC)
  # Make sure C++11/14 are enabled
  target_compile_options(ibex-geometry  PUBLIC ${PYBIND11_CPP_STANDARD})
endif()
target_link_libraries(ibex-geometry ${LIBS})

# INSTALL( TARGETS ibex-geometry DESTINATION lib)
# install(FILES ${HEADERS} DESTINATION "include/ibex-geometry")


PYBIND11_ADD_MODULE(geometry SHARED
    pyibex_geometry.cpp

)
target_link_libraries(geometry PRIVATE ${LIBS} ibex-geometry)

add_custom_command(TARGET geometry POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:geometry>" "${PYTHON_PACKAGE_DIR}/pyibex/geometry"
)
