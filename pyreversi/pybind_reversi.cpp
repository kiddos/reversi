#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "../src/game.h"
#include "../src/board.h"
#include "../src/ai.h"

namespace py = pybind11;
using namespace reversi;

PYBIND11_MODULE(reversi_gym, m) {
    py::class_<Move>(m, "Move")
        .def(py::init<>())
        .def(py::init<int, int>())
        .def_readwrite("x", &Move::x)
        .def_readwrite("y", &Move::y);

    py::class_<Board>(m, "Board")
        .def(py::init<>())
        .def("isvalidmove", py::overload_cast<const Move>(&Board::isvalidmove, py::const_))
        .def("getvalidmoves", &Board::getvalidmoves)
        .def("perform", &Board::perform)
        .def("getturn", &Board::getturn)
        .def("getblackcount", &Board::getblackcount)
        .def("getwhitecount", &Board::getwhitecount)
        .def("isfull", &Board::isfull)
        .def("get_state", [](const Board& b) {
            int board_arr[BOARDSIZE][BOARDSIZE];
            b.copyboard(board_arr);
            py::array_t<int> arr({BOARDSIZE, BOARDSIZE});
            auto buf = arr.request();
            int* ptr = static_cast<int*>(buf.ptr);
            for(int i=0; i<BOARDSIZE; i++)
                for(int j=0; j<BOARDSIZE; j++)
                    ptr[i*BOARDSIZE + j] = board_arr[i][j];
            return arr;
        });

    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def("playerinput", &Game::playerinput)
        .def("airespond", &Game::airespond, py::arg("smart") = false)
        .def("isgameover", &Game::isgameover)
        .def("getwinner", &Game::getwinner)
        .def("getboard", &Game::getboard);

    m.attr("WHITE") = Board::WHITE;
    m.attr("BLACK") = Board::BLACK;
    m.attr("EMPTY") = Board::EMPTY;
    m.attr("BOARDSIZE") = BOARDSIZE;
}
