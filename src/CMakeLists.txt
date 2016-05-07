set(SOURCE main.cpp window.cpp board.cpp game.cpp ai.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DDEBUG -g -O2")

add_executable(reversi ${SOURCE})
target_link_libraries(reversi allegro allegro_primitives)

