# Queen's Trace — N-Queens Backtracking Visualizer & C++17 Solver

Interactive web visualizer and C++17 backtracking solver for the generalized **N-Queens Constraint Satisfaction Problem**, combining the consolidated single-file `main.cpp` engine (`siddw09/queentrace`) with a modern dark-themed visual interface modeled after [n-queen-five.vercel.app](https://n-queen-five.vercel.app/).

## Key Capabilities

- **Exact C++17 Backtracking Engine (`main.cpp`)**:
  - **1D Permutation State Vector (`queens_[row] = col`)**: Eliminates row conflicts by construction and bounds auxiliary call-stack memory to $O(N)$.
  - **Unified Column & Diagonal Pruning (`isSafe`)**: Evaluates column equality (`c == col`) and $45^\circ$ main/anti-diagonal slope equality (`abs(r - row) == abs(c - col)`) against placed rows $r \in [0, \text{row} - 1]$.
  - **Dual Execution Modes**: Supports early-exit single-solution search (`solveOne` returning `optional<Solution>`) and exhaustive enumeration (`solveAll` returning `vector<Solution>`).
  - **Live `SearchStats` Telemetry**: Tracks `recursiveCalls`, `candidatePlacementsAttempted`, `successfulPlacements`, `backtracks`, `totalSolutionsFound`, and `durationMilliseconds`.
  - **Dual CLI & JSON API Interface**: Runs interactively in the terminal (`./queentrace`) or emits structured JSON with step-by-step trace events (`./queentrace --api <N> <mode>`).
- **Redesigned Interactive Web UI**:
  - **Home View**: Features the top navigation bar, a static $6 \times 6$ chessboard (`[1, 3, 5, 0, 2, 4]`) with cream (`#eeeed2`) and sage-green (`#769656`) squares, steel-blue display typography, and a blue-to-cyan gradient **`Visualize ->`** call-to-action button.
  - **Visualize View**: Features the **Board Size** slider (`1x1` to `8x8`), **Speed** slider (`Slow`, `Medium`, `Fast`, `Lighting Fast`), bold **`X Possible Solutions`** and **`Y Solutions Found`** counters, live `SearchStats` telemetry bar, 5 pill controls (`|<`, green `Start`, crimson `Stop`, orange `Solutions`, `>|`), and a 3-column **Discovered Solutions** miniature chessboard gallery.

## Technology Stack

| Layer | Technology | Role |
| :--- | :--- | :--- |
| **Core Solver** | C++17 (`vector`, `optional`, `chrono`, `algorithm`) | Backtracking state-space search, pruning, and telemetry |
| **Backend API Bridge** | Python 3 (`server.py`, `http.server` + `subprocess`) | Serves frontend and invokes `./queentrace --api <N> <mode>` on `POST /api/solve` |
| **Frontend UI** | HTML5, CSS3, Tailwind CSS, Vanilla ES6 JavaScript | Responsive chessboard rendering, step-by-step playback, and miniature board gallery |

## Project Structure

| File / Directory | Description |
| :--- | :--- |
| `main.cpp` | Consolidated C++17 N-Queens solver supporting both interactive CLI menu and `--api <N> <mode>` JSON output. |
| `server.py` | Zero-dependency Python HTTP server and `/api/solve` C++17 execution bridge. |
| `public/index.html` | Main HTML entry point containing the `Home` and `Visualize` views. |
| `public/styles.css` | Custom dark theme, cream/sage chessboard styling, slider tracks, and pill button states. |
| `public/app.js` | Frontend controller, step-trace playback engine, and `/api/solve` integration. |
| `queentrace.html` | Standalone single-file edition with all CSS and JS inlined for zero-setup browser execution. |

## Prerequisites

- **C++17 Compiler**: `g++` (GCC 7+ or Clang 5+)
- **Python 3**: Python 3.8+ (for running `server.py` locally)
- **Modern Web Browser**: Chrome, Firefox, Edge, or Safari

## Installation & Startup Instructions

### Option A: Run Standalone in Any Browser (Zero Setup)
Double-click `queentrace.html` (or `public/index.html`) to open the application directly in your browser.

### Option B: Run Full-Stack with the C++17 Backend Server
1. **Compile `main.cpp`**:
   ```bash
   g++ -std=c++17 -O2 main.cpp -o queentrace
   ```
2. **Start the local HTTP & API server**:
   ```bash
   python3 server.py
   ```
3. **Open the application**:
   Visit `http://localhost:8080` in your browser.

### Option C: Run Interactive C++17 Console Menu
```bash
./queentrace
```

## REST API Contract (`POST /api/solve`)

- **Request**:
  ```json
  {
    "boardSize": 4,
    "mode": "all"
  }
  ```
- **Response**:
  ```json
  {
    "success": true,
    "boardSize": 4,
    "mode": "all",
    "solutionCount": 2,
    "solutions": [[1, 3, 0, 2], [2, 0, 3, 1]],
    "stats": {
      "recursiveCalls": 17,
      "candidatePlacementsAttempted": 60,
      "successfulPlacements": 16,
      "backtracks": 16,
      "totalSolutionsFound": 2,
      "durationMilliseconds": 0.0038
    },
    "trace": [
      {"type": "place", "row": 0, "col": 0, "solutionsSoFar": 0, "board": [0, -1, -1, -1]}
    ]
  }
  ```

## Verification Summary ($N = 1$ to $8$)

| Board Size ($N$) | Expected Solutions $S(N)$ | C++17 & UI Verified Count | Recursive Calls (`solveAll`) | Candidate Checks (`isSafe`) | Backtracks Executed |
| :---: | :---: | :---: | ---: | ---: | ---: |
| **1 &times; 1** | 1 | **1** | 2 | 1 | 1 |
| **2 &times; 2** | 0 | **0** | 3 | 6 | 2 |
| **3 &times; 3** | 0 | **0** | 6 | 18 | 5 |
| **4 &times; 4** | 2 | **2** | 17 | 60 | 16 |
| **5 &times; 5** | 10 | **10** | 54 | 220 | 53 |
| **6 &times; 6** | 4 | **4** | 153 | 894 | 152 |
| **7 &times; 7** | 40 | **40** | 552 | 3,584 | 551 |
| **8 &times; 8** | 92 | **92** | 2,057 | 15,720 | 2,056 |
