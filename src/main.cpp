#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <ctime>
#include <climits>

#include "solver/Solver.h"
#include "utils/Scrambler.h"

using namespace std;

int main()
{
    Solver solver;
    Scrambler scrambler;

    ofstream bench("docs/benchmarks.md", ios::app);

    auto now = chrono::system_clock::now();
    time_t nowTime = chrono::system_clock::to_time_t(now);

    bench << "\n---\n\n";
    bench << "# Benchmark Run\n\n";
    bench << "**Date:** "
          << put_time(localtime(&nowTime), "%Y-%m-%d %H:%M:%S")
          << "\n\n";

    cout << "\n=========================================\n";
    cout << " RUBIK'S CUBE SOLVER BENCHMARK\n";
    cout << "=========================================\n\n";

    bench << "=========================================\n";
    bench << "RUBIK'S CUBE SOLVER BENCHMARK\n";
    bench << "=========================================\n\n";

    // ======================
// Benchmark Configuration
// ======================
const int START_DEPTH = 11;
const int END_DEPTH = 14;
const int TESTS_PER_DEPTH = 5;
const bool SAVE_RESULTS = true;

const int testsPerDepth = TESTS_PER_DEPTH;

    for (int depth = START_DEPTH; depth <= END_DEPTH; depth++)
    {
        cout << "#########################################\n";
        cout << "SCRAMBLE LENGTH = " << depth << "\n";
        cout << "#########################################\n\n";

        bench << "## Scramble Length = "
              << depth
              << "\n\n";

        bench << "| Test | Time (ms) | Nodes Expanded | Solution Length |\n";
        bench << "|----:|----------:|---------------:|----------------:|\n";

        long long totalTime = 0;
        long long totalNodes = 0;
        long long totalSolutionLength = 0;
        int solved = 0;

        long long maxNodes = -1;
        std::vector<Move> hardestScramble;

        long long bestTime = LLONG_MAX;
        long long worstTime = 0;

        for (int test = 1; test <= testsPerDepth; test++)
        {
            Cube cube;

            vector<Move> scramble =
                scrambler.generateScramble(depth);

            for (Move m : scramble)
                cube.applyMove(m);

            SolveResult result =
                solver.solve(cube);

            if (result.solved && result.nodesExpanded > maxNodes)
            {
                maxNodes = result.nodesExpanded;
                hardestScramble = scramble;
            }

            cout << "Test "
                 << setw(2)
                 << test
                 << " | ";

            if (result.solved)
            {
                solved++;

                totalTime += result.solveTimeMs;
                totalNodes += result.nodesExpanded;
                totalSolutionLength += result.solution.size();

                bestTime =
                    min(bestTime,
                        result.solveTimeMs);

                worstTime =
                    max(worstTime,
                        result.solveTimeMs);

                cout << "Time = "
                     << setw(6)
                     << result.solveTimeMs
                     << " ms   ";

                cout << "Nodes = "
                     << setw(10)
                     << result.nodesExpanded
                     << "   ";

                cout << "Len = "
                     << result.solution.size();

                bench << "| "
                      << test
                      << " | "
                      << result.solveTimeMs
                      << " | "
                      << result.nodesExpanded
                      << " | "
                      << result.solution.size()
                      << " |\n";
            }
            else
            {
                cout << "FAILED";

                bench << "| "
                      << test
                      << " | FAILED | FAILED | FAILED |\n";
            }

            cout << endl;
        }

        cout << "\n------------- SUMMARY -------------\n";

        cout << "Solved            : "
             << solved
             << "/"
             << testsPerDepth
             << endl;

        bench << "\n### Summary\n\n";

        bench << "- Solved: "
              << solved
              << "/"
              << testsPerDepth
              << "\n";

        if (solved)
        {
            cout << "Average Time      : "
                 << totalTime / solved
                 << " ms\n";

            cout << "Best Time         : "
                 << bestTime
                 << " ms\n";

            cout << "Worst Time        : "
                 << worstTime
                 << " ms\n";

            cout << "Average Nodes     : "
                 << totalNodes / solved
                 << endl;

            cout << "Average Sol Length: "
                 << fixed
                 << setprecision(2)
                 << (double)totalSolutionLength / solved
                 << endl;

            bench << "- Average Time: "
                  << totalTime / solved
                  << " ms\n";

            bench << "- Best Time: "
                  << bestTime
                  << " ms\n";

            bench << "- Worst Time: "
                  << worstTime
                  << " ms\n";

            bench << "- Average Nodes: "
                  << totalNodes / solved
                  << "\n";

            bench << "- Average Solution Length: "
                  << fixed
                  << setprecision(2)
                  << (double)totalSolutionLength / solved
                  << "\n\n";
        }

        if (!hardestScramble.empty())
        {
            cout << "Hardest Scramble : ";
            for (Move m : hardestScramble)
                cout << moveToString(m) << ' ';
            cout << "\nHardest Nodes    : " << maxNodes << "\n";
        }

        cout << "\n\n";
    }

    bench.close();

    return 0;
}