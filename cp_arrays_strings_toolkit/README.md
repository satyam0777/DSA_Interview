# Arrays & Strings Toolkit for CP (Codeforces + LeetCode)

You're right to prioritize this — arrays, strings, pairs, and 2D grids
aren't "one topic among many," they're the SUBSTRATE almost every other
topic sits on top of. A DP problem is usually "an array/grid + a
recurrence." A graph problem is usually "adjacency represented as
vector<vector<int>>." A greedy problem is usually "sort an array of
pairs by some key." Get comfortable with the material in this folder
and everything downstream gets easier to implement.

## Folder map

1. `01_arrays/` — array vs vector, declaration/init, common operations,
   passing to functions, classic pitfalls (uninitialized values,
   off-by-one, out-of-bounds).
2. `02_strings/` — std::string as a mutable char array, indexing,
   substr, comparisons, building strings, char <-> int conversions,
   splitting/parsing input.
3. `03_pairs_tuples_vectors/` — pair, tuple, structured bindings,
   vector<pair<>>, sorting/searching with pairs, pair as a map key.
4. `04_grids_2d/` — 2D vectors, row/col indexing conventions, the
   direction-array pattern for grid traversal (flood fill, BFS on
   grids), boundary checks.
5. `05_maps_structs_objects/` — map vs unordered_map, map of vectors,
   map of pairs, and how to define your own "object" (struct) in CP —
   when to bother, when a pair/tuple is enough.
6. `06_dp_with_arrays_strings/` — how array/string problems become DP:
   1D dp over an array, 2D dp over two strings (LCS/edit distance),
   DP with pair-shaped state, memoization with a map.
7. `07_string_algorithms/` — frequency counting, palindrome checks,
   the KMP prefix function, polynomial rolling hash — the "advanced"
   string toolbox that shows up once you're past easy/medium problems.

## How to use it

Same as the main CP toolkit: read the comments, don't just copy the
code. Compile and run each file, then deliberately break things (change
a `<` to `<=`, remove a bounds check) to see what happens — that's the
fastest way to build the instinct for what a bug LOOKS like when you
hit it during a real contest.

```bash
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -o out file.cpp
./out
```

## The one big idea to hold onto

Almost every "hard to implement" feeling in CP comes down to: **what is
my state, and what container represents it?**
- A single index into an array → just an `int`.
- A position in a 2D grid → a `pair<int,int>` or two ints `(row, col)`.
- "Have I used these coins/items" → a bitmask `int`.
- "Best answer using first i elements with j selected" → `dp[i][j]`.

Once you can name your state and pick the right container for it
(array, pair, map, struct), the implementation is usually just
mechanical translation — which is exactly what this folder is for.
