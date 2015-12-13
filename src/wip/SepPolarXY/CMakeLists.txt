set( SRCS 
			ibex_SepPolarXY.cpp
		)
set( HEADERS
			ibex_SepPolarXY.cpp
		)

add_library(SepPolarXY STATIC ${SRCS} ${HEADERS})
target_link_libraries(SepPolarXY ${LIBS})

