set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DDEBUG -g -O2")

add_executable(boardtest boardtest.cpp ../src/board.cpp)
add_executable(gametest gametest.cpp ../src/board.cpp ../src/game.cpp ../src/ai.cpp)

add_test(boardtest ./boardtest)
add_test(gametest ./gametest)
