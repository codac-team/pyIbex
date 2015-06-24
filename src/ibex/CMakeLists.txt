
set( DIR_LIST 
	"geometry" 
	"set" 
	"alg"
	)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

foreach( dir ${DIR_LIST})
	message(STATUS ${dir})
	file(GLOB dir_headers ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/*.h)
	file(GLOB dir_src ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/*.cpp)
	INCLUDE_DIRECTORIES(${dir})
	message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/${dir})
	set(IBEX_ADDON_SRCS ${IBEX_ADDON_SRCS} ${dir_src})
	set(IBEX_ADDON_HEADERS ${IBEX_ADDON_HEADERS} ${dir_headers})
endforeach(dir)

set(IBEX_ADDON_SRCS ${IBEX_ADDON_SRCS} PARENT_SCOPE)
# add_library(ibex_extra SHARED ${IBEX_ADDON_SRCS} ${IBEX_ADDON_HEADERS})
# target_link_libraries( ibex_extra ${LIBS})
# install( TARGETS ibex_extra DESTINATION lib)







