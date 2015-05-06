INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/ibex/alg)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/ibex/set)
# INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/ibex/alg)

add_executable(ex_sepProj ex_sepProj.cpp ../src/ibex/alg/vibes.cpp)
target_link_libraries(ex_sepProj ${LIBS} ibex_extra)

add_executable(ex_sepProj2 ex_sepProj2.cpp ../src/ibex/alg/vibes.cpp)
target_link_libraries(ex_sepProj2 ${LIBS} ibex_extra)

add_executable(ex_sepProjSpheres ex_sepProjSpheres.cpp ../src/ibex/alg/vibes.cpp)
target_link_libraries(ex_sepProjSpheres ${LIBS} ibex_extra)
