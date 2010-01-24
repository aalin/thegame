cmake_minimum_required(VERSION 2.6)
 
project(thegame)
 
find_package(SDL)
find_package(OpenGL)
find_package(Boost)

include_directories(${OPENGL_INCLUDE_DIR} ${SDL_INCLUDE_DIR})
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wall -Wextra")
 
set(SHARED_SOURCES 
	engine
	heightmap
	cube_scene
	shader_program
	vector3
)

set(MAIN_SOURCES main ${SHARED_SOURCES})
if(APPLE)
        set(MAIN_SOURCES ${MAIN_SOURCES} sdl/SDLMain.m)
endif(APPLE)

set(TEST_SOURCES test ${SHARED_SOURCES})
 
add_executable(thegame ${MAIN_SOURCES})
add_executable(test ${TEST_SOURCES})
target_link_libraries(thegame ${SDL_LIBRARY} ${OPENGL_LIBRARIES})
target_link_libraries(test ${SDL_LIBRARY} ${OPENGL_LIBRARIES})
