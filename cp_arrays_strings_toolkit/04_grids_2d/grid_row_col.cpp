/*
    ============================================================
    2D GRIDS -- row/col conventions, traversal, flood fill / grid BFS
    ============================================================
    Concept: a 2D grid problem (matrix, maze, image) is basically a graph
    where each cell is a node and its neighbors are the cells up/down/
    left/right (sometimes diagonals too). Almost all grid problems reuse
    the SAME traversal skeleton -- learn it once, recognize it forever.
*/
#include <bits/stdc++.h>
using namespace std;

// ---------------------------------------------------------------
// ROW/COL CONVENTION -- pick one and stick to it (this file's convention:
// first index = row, second index = column, matching how the grid is
// usually READ from input line by line)
// ---------------------------------------------------------------
void gridBasics() {
    int rows = 3, cols = 4;
    vector<vector<int>> grid(rows, vector<int>(cols, 0));

    // access: grid[row][col], NOT grid[col][row] -- mixing these up is
    // a classic silent bug (works fine on square grids, breaks on
    // rectangular ones, so it can hide until a specific test case).
    grid[1][2] = 5; // row 1, col 2

    // reading a grid given as `rows` strings (very common input format):
    // vector<string> g(rows);
    // for (int i = 0; i < rows; ++i) cin >> g[i];
    // then g[i][j] is the character at row i, col j.

    // iterate every cell, row-major order (matches how it was read in)
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // process grid[r][c]
        }
    }
}

// ---------------------------------------------------------------
// THE DIRECTION-ARRAY PATTERN -- the single most reused snippet in
// grid problems. Instead of writing 4 near-identical if-statements for
// up/down/left/right, loop over parallel dr[]/dc[] arrays.
// ---------------------------------------------------------------
const int dr4[] = {-1, 1, 0, 0};   // up, down, left, right (row deltas)
const int dc4[] = {0, 0, -1, 1};   // up, down, left, right (col deltas)

// 8-directional (adds diagonals) -- used when diagonal moves are allowed
const int dr8[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dc8[] = {-1, 0, 1, -1, 1, -1, 0, 1};

bool inBounds(int r, int c, int rows, int cols) {
    // THE bounds check every grid problem needs -- forgetting this, or
    // getting the direction of a comparison wrong, is the #1 source of
    // out-of-bounds crashes in grid problems.
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

// ---------------------------------------------------------------
// FLOOD FILL / GRID BFS -- the canonical template. This exact skeleton
// solves: connected components, shortest path in a maze, "number of
// islands" (LeetCode classic), multi-source BFS, etc. -- just change
// what counts as a "valid" neighbor and what you do when you visit one.
// ---------------------------------------------------------------
int floodFillBFS(vector<vector<int>>& grid, int startR, int startC) {
    int rows = grid.size(), cols = grid[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    queue<pair<int,int>> q;
    q.push({startR, startC});
    visited[startR][startC] = true;
    int cellCount = 0;

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        cellCount++;

        for (int dir = 0; dir < 4; ++dir) {
            int nr = r + dr4[dir];
            int nc = c + dc4[dir];

            if (!inBounds(nr, nc, rows, cols)) continue;   // off the grid
            if (visited[nr][nc]) continue;                  // already seen
            if (grid[nr][nc] != grid[startR][startC]) continue; // problem-
                                                                    // specific
                                                                    // "is this
                                                                    // a valid
                                                                    // neighbor"
                                                                    // check --
                                                                    // adapt per
                                                                    // problem

            visited[nr][nc] = true;
            q.push({nr, nc});
        }
    }
    return cellCount;
}

// ---------------------------------------------------------------
// MULTI-SOURCE BFS -- push ALL sources into the queue before starting
// (e.g. "distance from the nearest 1 in a 0/1 grid" -- LeetCode 542).
// Works because BFS naturally expands layer by layer regardless of how
// many starting points you seed it with.
// ---------------------------------------------------------------
vector<vector<int>> multiSourceBFS(vector<vector<int>>& grid) {
    int rows = grid.size(), cols = grid[0].size();
    vector<vector<int>> dist(rows, vector<int>(cols, -1));
    queue<pair<int,int>> q;

    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            if (grid[r][c] == 1) { dist[r][c] = 0; q.push({r, c}); }

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        for (int dir = 0; dir < 4; ++dir) {
            int nr = r + dr4[dir], nc = c + dc4[dir];
            if (!inBounds(nr, nc, rows, cols)) continue;
            if (dist[nr][nc] != -1) continue;
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }
    return dist;
}

/*
    QUICK REFERENCE
    ----------------------------------------------------------
    grid[row][col]                     access convention (pick one, be
                                        consistent -- this file uses
                                        row-major: outer index = row)
    dr4[]/dc4[]                        4-directional delta arrays
    dr8[]/dc8[]                        8-directional (with diagonals)
    inBounds(r, c, rows, cols)         ALWAYS check before indexing
    flood fill / grid BFS               connected components, islands,
                                        shortest path in a maze
    multi-source BFS                    "distance to nearest X" problems
    ----------------------------------------------------------
    PITFALL CHECKLIST for grid problems:
    - [ ] Did you check bounds BEFORE indexing grid[nr][nc]? (checking
          after will crash/UB)
    - [ ] Did you mark a cell visited BEFORE or immediately when pushing
          it to the queue, not only when popped? (marking only on pop
          can push the same cell multiple times, wasting time/memory,
          or even causing wrong answers in some formulations)
    - [ ] rows vs cols swapped? grid.size() = number of rows,
          grid[0].size() = number of columns -- easy to mix up on
          non-square grids.
    - [ ] Are you using grid[0].size() on a possibly-EMPTY grid? Check
          rows > 0 first if the grid could be empty.
*/

int main() { return 0; }
