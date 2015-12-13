set( SRCS 
			paving.cpp
			pyIbex_Paving.cpp
		)
set( HEADERS
			paving.h
		)

add_library(Paving STATIC ${SRCS} ${HEADERS})
target_link_libraries(Paving ${LIBS})