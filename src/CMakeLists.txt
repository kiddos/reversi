set(SOURCE main.cpp window.cpp board.cpp game.cpp ai.cpp aitrainer.cpp)
set(HEADER window.h board.h game.h ai.h aitrainer.h)
set(NNET_SOURCE nnet/act.cpp nnet/layer.cpp nnet/nnet.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DDEBUG -g -O3 -Wall -Wextra -fopenmp")
set(ALLEGRO_LIBS allegro allegro_primitives allegro_ttf allegro_font)
set(NNET_LIBS armadillo)

add_executable(reversi ${SOURCE} ${NNET_SOURCE})
target_link_libraries(reversi ${ALLEGRO_LIBS} ${NNET_LIBS})

file(COPY ${CMAKE_SOURCE_DIR}/font DESTINATION ${CMAKE_BINARY_DIR})

