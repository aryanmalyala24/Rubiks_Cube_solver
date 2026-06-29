cmake_minimum_required(VERSION 3.10)

project(RubiksCubeSolver)

set(CMAKE_CXX_STANDARD 17)


add_executable(
    RubiksCubeSolver

    src/main.cpp

    src/cube/Cube.cpp
    src/cube/Moves.cpp

    src/database/StateDatabase.cpp

    src/search/Node.cpp
    src/search/MoveGenerator.cpp
    src/search/Heuristic.cpp
    src/search/DFS.cpp
    src/search/IDDFS.cpp
    src/search/IDAStar.cpp

    src/solver/Solver.cpp
    src/solver/DatabaseSolver.cpp

    src/utils/Scrambler.cpp

)

target_link_libraries(
    RubiksCubeSolver
    raylib
)