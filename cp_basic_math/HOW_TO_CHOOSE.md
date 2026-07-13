# HOW TO CHOOSE THE RIGHT TECHNIQUE

This is the piece nobody writes down clearly enough: **how do you look at a
problem and know what to reach for?** This is a signal → technique lookup
table built from patterns that repeat constantly in SDE interviews (LeetCode)
and CF contests. Read top to bottom like a flowchart when you're stuck.

---

## Step 0: Read the constraints FIRST

Before thinking about the approach, look at input size (n) — it tells you the
required time complexity, which massively narrows down valid techniques.

| n (input size) | Required complexity | Techniques that fit |
|---|---|---|
| n ≤ 10-12 | O(2ⁿ), O(n!) | Brute force, backtracking, bitmask DP |
| n ≤ 20-22 | O(2ⁿ · n) | Bitmask DP (subsets as bitmasks) |
| n ≤ 500 | O(n³) | 3-nested loops, Floyd-Warshall, simple DP over pairs |
| n ≤ 5,000 | O(n²) | Nested loops, simple DP, brute force pairs |
| n ≤ 10⁵-10⁶ | O(n log n) | Sorting, binary search, heap, segment tree, most greedy |
| n ≤ 10⁷-10⁸ | O(n) | Single pass, two pointers, sliding window, prefix sum |
| n up to 10¹⁸ | O(log n) | Binary search on answer, fast exponentiation, digit DP |

**If your first idea is O(n²) but n is 10⁵ — that idea is wrong. Stop and find the O(n log n) approach before coding.**

---

## Step 1: Keyword → Technique lookup

| Keywords / phrasing in the problem | Likely technique | File to reference |
|---|---|---|
| "subarray", "substring", "contiguous" | Sliding window / prefix sum | `01_arrays/sliding_window.cpp`, `prefix_sum.cpp` |
| "two elements that sum to..." on **sorted** array | Two pointers | `01_arrays/two_pointers.cpp` |
| "minimize the maximum" / "maximize the minimum" / "smallest X such that..." | Binary search on the answer | `02_binary_search/binary_search_patterns.cpp` |
| "shortest path", "minimum steps", **unweighted** graph | BFS | `03_graphs/bfs_dfs.cpp` |
| "shortest path", **weighted**, non-negative | Dijkstra | `03_graphs/dijkstra.cpp` |
| "connected components", "are these connected", "merge groups" | Union-Find (DSU) | `03_graphs/union_find.cpp` |
| "prerequisites", "order of tasks", "dependencies" | Topological sort | `03_graphs/topological_sort.cpp` |
| "count ways to...", "maximum/minimum value achievable", overlapping subproblems | Dynamic Programming | `04_dp/knapsack_lis.cpp` |
| "count numbers in range [L,R] with digit property" | Digit DP | `04_dp/digit_dp.cpp` |
| "...mod 10^9+7" | Modular arithmetic (fast power, mod inverse) | `05_number_theory/sieve_modpow.cpp` |
| "range sum/min/max query" + "update a value" repeatedly | Segment tree / Fenwick tree | `06_trees/segment_tree.cpp`, `fenwick_tree.cpp` |
| "find all occurrences of a pattern in text" | KMP / Z-function | `07_strings/kmp_zfunction.cpp` |
| "sort by a custom rule", "sort by second value", "break ties by..." | Custom comparator | `15_comparators/comparators.cpp` |
| "count subsets", "iterate all subsets of a mask", "n ≤ 20 and needs state over a set" | Bitmask DP / subset enumeration | `16_bit_manipulation/bit_manipulation.cpp` |
| "number of ways...", "choose r from n", "coprime", "nth term of a recurrence for huge n" | Combinatorics / number theory / matrix exponentiation | `17_math_for_cp/math_for_cp.cpp` |
| "generate all subsets/permutations/combinations", "all valid arrangements" | Backtracking | `09_recursion_backtracking/backtracking.cpp` |
| "at each step pick the best local choice", "activity selection", "scheduling to maximize count" | Greedy | `10_greedy/greedy.cpp` |
| "kth largest/smallest", "top k elements", "merge k sorted lists" | Heap (priority queue) | `11_heaps/heap_priority_queue.cpp` |
| "reverse a linked list", "detect cycle", "merge two sorted lists" | Linked list techniques (fast/slow pointers) | `12_linked_list/linked_list.cpp` |
| "binary tree", "BST", "lowest common ancestor", "level order" | Tree traversal (DFS/BFS on trees) | `13_binary_trees/binary_tree_bst.cpp` |
| "sort this", or sorting is a preprocessing step | Pick the right sort | `14_sorting_algorithms/sorting_algorithms.cpp` |

---

## Step 2: The "is it DP?" test

Ask these two questions. If both are YES, it's a DP problem:
1. **Optimal substructure**: can the answer for the whole problem be built from
   answers to smaller versions of the same problem?
2. **Overlapping subproblems**: does a naive recursive solution recompute the
   same smaller problem many times?

If yes to both → define `dp[state] = ...`, find the recurrence, code it
bottom-up (iterative) or top-down (memoized recursion).

**If it's optimal substructure WITHOUT overlapping subproblems** → that's
usually **greedy**, not DP (e.g., activity selection, Huffman coding).

---

## Step 3: Greedy vs DP — how to tell them apart

Greedy makes ONE irrevocable locally-optimal choice at each step and never
looks back. It only works when a **greedy-choice property** holds: the
locally optimal choice is always part of *some* globally optimal solution.

- If you can construct a counter-example where the "obviously best" local
  choice leads to a worse overall answer → it's NOT greedy, it's DP.
- Rule of thumb: if the problem says "maximum/minimum **number of** something"
  and a greedy idea seems to work, TRY TO BREAK IT with a small example before
  trusting it. If you can't break it in 2 minutes, it's probably genuinely greedy.

---

## Step 4: Recursion → is backtracking needed?

If the problem asks you to **enumerate/generate all** valid configurations
(all subsets, all permutations, all valid parenthesizations, N-Queens, Sudoku),
it's backtracking: recursion + explicit "undo the choice" step + pruning.

The three-part backtracking template (see `09_recursion_backtracking/backtracking.cpp`):
```
choose  -> pick a candidate
explore -> recurse deeper with that candidate fixed
unchoose -> undo the pick before trying the next candidate (the "backtrack")
```

---

## Step 5: Data structure signals (SDE interview favorites)

| Need | Data structure |
|---|---|
| Fast lookup by key | `unordered_map` / hash map |
| Maintain sorted order + fast insert/search | `set` / `map` (balanced BST, O(log n)) |
| Always get min/max quickly, allow insert | `priority_queue` (heap) |
| LIFO — undo, matching brackets, DFS iterative | `stack` |
| FIFO — BFS, task queues | `queue` / `deque` |
| Need both stack behavior AND fast max — sliding window max | `deque` (monotonic deque) |
| Group elements by some key, count frequency | `unordered_map<key, count>` |

---

## Step 6: Practical interview process (SDE rounds)

1. **Clarify constraints out loud**: input size, can values be negative, are
   there duplicates, is the array sorted, what to return on empty input.
2. **State brute force first** ("the obvious O(n²) way is...") — shows you
   understand the problem, then say what's slow about it.
3. **Name the pattern before coding**: "this looks like a sliding window
   problem because we need a contiguous subarray satisfying a sum condition."
4. **Code it using the templates in this repo** — the boilerplate should be
   muscle memory so you spend your thinking time on the actual logic.
5. **Trace through the example by hand** before saying "I'm done."
6. **State final complexity** (time AND space) unprompted at the end.

---

## Quick self-test

Next time you read a new problem, before opening an editor, answer:
1. What's n, and what complexity does that imply?
2. Which row in the Step 1 table does this match?
3. Is it DP (optimal substructure + overlapping subproblems) or greedy
   (irrevocable local choice)?
4. What data structure does the "need" in Step 5 point to?

If you can answer these four in under a minute, you already know *what* to
write — the rest is just careful implementation using the patterns in this repo.
