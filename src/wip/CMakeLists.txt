set ( wip_dirs 
				Paving
)


foreach(MODULE ${wip_dirs})
	add_subdirectory(${MODULE})
	INCLUDE_DIRECTORIES(${MODULE})	
	set (wip_lib ${wip_lib} ${MODULE})
  set(WIP_SRCS ${WIP_SRCS} "${MODULE}/pyIbex_${MODULE}.cpp")
endforeach()

add_library(wip ${WIP_SRCS})
target_link_libraries(wip ${LIBS} ${wip_lib})

#install( TARGETS wip DESTINATION "${PYTHON_INSTDIR}/pyIbex")


