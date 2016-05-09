set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DDEBUG -g -O2")
set(NNET_SOURCE ../src/nnet/act.cpp ../src/nnet/layer.cpp ../src/nnet/nnet.cpp)

add_executable(boardtest boardtest.cpp ../src/board.cpp)
add_executable(gametest gametest.cpp ../src/board.cpp ../src/game.cpp ../src/ai.cpp ${NNET_SOURCE})

add_test(boardtest ./boardtest)
add_test(gametest ./gametest)

target_link_libraries(gametest armadillo)
