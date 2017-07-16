# set( SRCS
      # pyibex_GeoImages.cpp
  # )

set( HEADERS
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_GeoImage.h

  )
include_directories(.)
# FILE(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

# message(STATUS ${SRCS} ${HEADERS})
# add_library(pyibex-image ${SRCS} ${HEADERS})
# if(NOT MSVC)
#   # Make sure C++11/14 are enabled
#   target_compile_options(pyibex-image  PUBLIC ${PYBIND11_CPP_STANDARD})
# endif()
# target_link_libraries(pyibex-image ${LIBS})

# INSTALL( TARGETS pyibex-image DESTINATION lib)
# install(FILES ${HEADERS} DESTINATION "include/pyibex-image")


PYBIND11_ADD_MODULE(image SHARED
    pyibex_images.cpp
)
target_link_libraries(image PRIVATE ${LIBS})

add_custom_command(TARGET image POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:image>" "${PYTHON_PACKAGE_DIR}/pyibex/image"
)
