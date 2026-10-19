from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "reversi_gym",
        ["pybind_reversi.cpp", "../src/game.cpp", "../src/board.cpp", "../src/ai.cpp", "../src/nnet/nnet.cpp", "../src/nnet/layer.cpp", "../src/nnet/act.cpp"],
        include_dirs=[pybind11.get_include(), "../src"],
        language='c++',
        extra_compile_args=['-std=c++14', '-O3', '-Wall', '-fopenmp'],
        extra_link_args=['-fopenmp', '-larmadillo'],
    ),
]

setup(
    name="reversi_gym",
    ext_modules=ext_modules,
)
