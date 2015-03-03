
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pyIbex_Setting.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/pyIbex_Setting.h
)
include_directories(".")
include_directories("core")
file(GLOB CORE_SRCS "core/pyIbex_*.cpp")

if(WITH_VIBES)
	include_directories(alg)
	set(ALG_SRCS ${ALG_SRCS} alg/vibes.cpp alg/pyIbex_SIVIA.cpp)
endif(WITH_VIBES)

PYTHON_ADD_MODULE(pyIbex pyIbex.cpp ${CORE_SRCS} ${ALG_SRCS})
target_link_libraries(pyIbex ${LIBS})

install( TARGETS pyIbex DESTINATION ${PYTHON_INSTDIR})



