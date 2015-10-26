set( SRCS 
			paving.cpp
			pyIbex_Paving.cpp
		)
set( HEADERS
			paving.h
		)

add_library(Paving STATIC ${SRCS} ${HEADERS})
target_link_libraries(Paving ${LIBS})

#add_executable(dbg1 ex_dbg1.cpp vibes.cpp vibes.h)
#target_link_libraries(dbg1 ${LIBS} Paving)
