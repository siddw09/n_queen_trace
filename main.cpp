#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;

class NQueens {
private:
    int n;
    vector<int> board;              // board[row] stores the column index of the queen
    vector<vector<int>> solutions;  // Stores all valid solutions

    // Search statistics
    long long recursiveCalls = 0;
    long long placementsTested = 0;
    long long queensPlaced = 0;
    long long backtracks = 0;
    double durationMs = 0.0;

    // Check if placing a queen at (row, col) is safe from previous rows
    bool isSafe(int row, int col) const {
        for (int prevRow = 0; prevRow < row; ++prevRow) {
            int prevCol = board[prevRow];
            if (prevCol == col || abs(row - prevRow) == abs(col - prevCol)) {
                return false; // Same column or diagonal conflict
            }
        }
        return true;
    }

    // Recursive backtracking helper
    bool backtrack(int row, bool stopAtFirst) {
        recursiveCalls++;

        if (row == n) {
            solutions.push_back(board);
            return stopAtFirst;
        }

        for (int col = 0; col < n; ++col) {
            placementsTested++;
            if (isSafe(row, col)) {
                board[row] = col; // Place queen
                queensPlaced++;

                if (backtrack(row + 1, stopAtFirst)) {
                    return true;
                }

                board[row] = -1;  // Backtrack (remove queen)
                backtracks++;
            }
        }
        return false;
    }

    void reset(int newSize) {
        n = newSize;
        board.assign(n, -1);
        solutions.clear();
        recursiveCalls = 0;
        placementsTested = 0;
        queensPlaced = 0;
        backtracks = 0;
        durationMs = 0.0;
    }

public:
    NQueens(int size = 0) : n(size), board(size, -1) {}

    // Print bordered chessboard and column array
    void printBoard(const vector<int>& cols) const {
        string border = "+" + string(n * 3, '-') + "+\n";
        cout << border;
        for (int r = 0; r < n; ++r) {
            cout << "|";
            for (int c = 0; c < n; ++c) {
                cout << (cols[r] == c ? " Q " : " . ");
            }
            cout << "|\n";
        }
        cout << border;

        cout << "Column positions: [";
        for (int i = 0; i < n; ++i) {
            cout << cols[i] << (i + 1 < n ? ", " : "");
        }
        cout << "]\n";
    }

    void solveOne(int size) {
        reset(size);
        auto start = chrono::high_resolution_clock::now();
        bool found = backtrack(0, true);
        auto end = chrono::high_resolution_clock::now();
        durationMs = chrono::duration<double, milli>(end - start).count();

        if (found) {
            cout << "\n[SUCCESS] Solution found for N = " << n << ":\n";
            printBoard(solutions[0]);
        } else {
            cout << "\n[RESULT] No solution exists for N = " << n << ".\n";
            if (n == 2 || n == 3) {
                cout << "Note: Mathematically, no non-attacking configuration exists for N = " << n << ".\n";
            }
        }
        printStats("Find ONE Solution");
    }

    void solveAll(int size) {
        reset(size);
        auto start = chrono::high_resolution_clock::now();
        backtrack(0, false);
        auto end = chrono::high_resolution_clock::now();
        durationMs = chrono::duration<double, milli>(end - start).count();

        int total = static_cast<int>(solutions.size());
        cout << "\n[RESULT] Found " << total << " solution(s) for N = " << n << ".\n";

        if (total == 0) {
            if (n == 2 || n == 3) {
                cout << "Note: Mathematically, no non-attacking configuration exists for N = " << n << ".\n";
            }
        } else {
            int displayLimit = total;
            if (total > 5) {
                cout << "Enter number of solutions to display (1 to " << total << ", or 0 for ALL): ";
                int userLimit = 0;
                if (cin >> userLimit && userLimit > 0 && userLimit <= total) {
                    displayLimit = userLimit;
                }
            }

            cout << "\nDisplaying " << displayLimit << " of " << total << " solution(s):\n";
            for (int i = 0; i < displayLimit; ++i) {
                cout << "\nSolution " << (i + 1) << " of " << total << ":\n";
                printBoard(solutions[i]);
            }
            if (displayLimit < total) {
                cout << "\n(" << (total - displayLimit) << " additional solution(s) omitted)\n";
            }
        }
        printStats("Find ALL Solutions");
    }

    void printStats(const string& searchType) const {
        if (n <= 0) {
            cout << "\nNo search has been performed yet! Run option 1 or 2 first.\n";
            return;
        }
        cout << "\n================ SEARCH STATISTICS ================\n"
             << left << setw(28) << "Search Type:" << searchType << "\n"
             << left << setw(28) << "Board Size (N x N):" << n << "\n"
             << left << setw(28) << "Solutions Found:" << solutions.size() << "\n"
             << left << setw(28) << "Recursive Calls:" << recursiveCalls << "\n"
             << left << setw(28) << "Candidate Positions Tested:" << placementsTested << "\n"
             << left << setw(28) << "Successful Placements:" << queensPlaced << "\n"
             << left << setw(28) << "Backtracks Executed:" << backtracks << "\n"
             << fixed << setprecision(4)
             << left << setw(28) << "Search Duration:" << durationMs << " ms\n"
             << "===================================================\n";
    }

    static void explainAlgorithm() {
        cout << "\n================ N-QUEENS BACKTRACKING ================\n"
             << "1. Goal: Place N non-attacking queens on an N x N board.\n"
             << "2. Row-by-Row Strategy: Place 1 queen per row (0 to N-1).\n"
             << "3. Safety Check for (row, col) against previous rows (r, c):\n"
             << "   - Column conflict:   c == col\n"
             << "   - Diagonal conflict: abs(row - r) == abs(col - c)\n"
             << "4. Backtracking: If a row has no valid column, undo the\n"
             << "   previous row's queen (board[row] = -1) and try the next column.\n"
             << "5. Complexity: O(N!) worst-case time, O(N) recursion stack space.\n"
             << "=======================================================\n";
    }
};

int main() {
    NQueens solver;
    string lastSearchType = "None";

    cout << "======================================================\n"
         << "     QueenTrace: N-Queens Puzzle Solver (C++ STL)     \n"
         << "======================================================\n";

    while (true) {
        cout << "\n---------------- MAIN MENU ----------------\n"
             << " 1. Find ONE solution\n"
             << " 2. Find ALL solutions\n"
             << " 3. Explain the algorithm\n"
             << " 4. Display search statistics\n"
             << " 0. Exit\n"
             << "-------------------------------------------\n"
             << "Select an option (0-4): ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number between 0 and 4.\n";
            continue;
        }

        if (choice == 0) {
            cout << "\nExiting N-Queens Solver. Thank you!\n";
            break;
        }

        if (choice == 1 || choice == 2) {
            int n;
            cout << "Enter chessboard size N (1 to 16): ";
            if (!(cin >> n) || n < 1 || n > 16) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid size! Please enter an integer between 1 and 16.\n";
                continue;
            }
            if (choice == 1) {
                lastSearchType = "Find ONE Solution";
                solver.solveOne(n);
            } else {
                lastSearchType = "Find ALL Solutions";
                solver.solveAll(n);
            }
        } else if (choice == 3) {
            NQueens::explainAlgorithm();
        } else if (choice == 4) {
            solver.printStats(lastSearchType);
        } else {
            cout << "Invalid choice! Please select 0 to 4.\n";
        }
    }

    return 0;
}