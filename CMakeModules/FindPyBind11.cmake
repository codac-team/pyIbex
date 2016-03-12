# findpybind11.cmake


INCLUDE_DIRECTORIES (pybind11/include)


FUNCTION(PYBIND11_ADD_MODULE _NAME)
  add_library( ${_NAME} ${ARGN})

  # Don't add a 'lib' prefix to the shared library
  set_target_properties(${_NAME} PROPERTIES PREFIX "")

  if (WIN32)
    if (MSVC)
      # Enforce size-based optimization and link time code generation
      # on MSVC (~30% smaller binaries in experiments). /bigobj is needed
      # for bigger binding projects due to the limit to 64k addressable sections
      # /MP enables multithreaded builds (relevant when there are many files).
      set_target_properties(${_NAME} PROPERTIES COMPILE_FLAGS "/Os /GL /MP /bigobj")
      set_target_properties(${_NAME} PROPERTIES LINK_FLAGS "/LTCG")
    endif()

    # .PYD file extension on Windows
    set_target_properties(${_NAME} PROPERTIES SUFFIX ".pyd")

    # Link against the Python shared library
    #target_link_libraries(example ${PYTHON_LIBRARY})
  elseif (UNIX)
    # It's quite common to have multiple copies of the same Python version
    # installed on one's system. E.g.: one copy from the OS and another copy
    # that's statically linked into an application like Blender or Maya.
    # If we link our plugin library against the OS Python here and import it
    # into Blender or Maya later on, this will cause segfaults when multiple
    # conflicting Python instances are active at the same time (even when they
    # are of the same version).
  
    # Windows is not affected by this issue since it handles DLL imports 
    # differently. The solution for Linux and Mac OS is simple: we just don't
    # link against the Python library. The resulting shared library will have
    # missing symbols, but that's perfectly fine -- they will be resolved at
    # import time.

    # .SO file extension on Linux/Mac OS
    set_target_properties(${_NAME} PROPERTIES SUFFIX ".so")

    # Optimize for a small binary size
    if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
      set_target_properties(${_NAME} PROPERTIES COMPILE_FLAGS "-Os")
    endif()

    # Strip unnecessary sections of the binary on Linux/Mac OS
    # if(APPLE)
    #   set_target_properties(${_NAME} PROPERTIES MACOSX_RPATH ".")
    #   set_target_properties(${_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup ")
    #   if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
    #     add_custom_command(TARGET ${_NAME} POST_BUILD COMMAND strip -u -r ${PROJECT_SOURCE_DIR}/${_NAME}/${_NAME}.so)
    #   endif()
    # else()
    #   if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
    #     add_custom_command(TARGET ${_NAME} POST_BUILD COMMAND strip ${PROJECT_SOURCE_DIR}/${_NAME}/${_NAME}.so)
    #   endif()
    # endif()
  endif()
endfunction()