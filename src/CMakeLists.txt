cmake_minimum_required(VERSION 2.6)
 
project(thegame)
 
find_package(SDL)
find_package(SDL_image)
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
	util
	image
)

set(MAIN_SOURCES main ${SHARED_SOURCES})
if(APPLE)
        set(MAIN_SOURCES ${MAIN_SOURCES} sdl/SDLMain.m)
endif(APPLE)

set(TEST_SOURCES test ${SHARED_SOURCES})
 
add_executable(thegame ${MAIN_SOURCES})
add_executable(test ${TEST_SOURCES})
set(SHARED_LIBRARIES ${SDL_LIBRARY} ${SDL} ${OPENGL_LIBRARIES} ${SDLIMAGE_LIBRARY})
target_link_libraries(thegame ${SHARED_LIBRARIES})
target_link_libraries(test ${SHARED_LIBRARIES})
