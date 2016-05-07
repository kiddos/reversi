set(SOURCE main.cpp window.cpp board.cpp game.cpp ai.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DDEBUG -g -O2 -Wall")
set(ALLEGRO_LIBS allegro allegro_primitives allegro_ttf allegro_font)

add_executable(reversi ${SOURCE})
target_link_libraries(reversi ${ALLEGRO_LIBS})

file(COPY ${CMAKE_SOURCE_DIR}/font DESTINATION ${CMAKE_BINARY_DIR})

