# Common Pitfalls Cheat Sheet — Arrays, Strings, Pairs, Maps, Grids

Keep this open in a tab during your first 15-20 contests. Every item here
is a bug that WILL cost you time if you don't already have it memorized.

## Arrays / Vectors
- `a.size()` returns `size_t` (unsigned). `a.size() - 1` when `a` is
  empty underflows to a huge number. Cast to `int` before subtracting,
  or guard with `if (!a.empty())`.
- `vector<int> a(n)` value-initializes to 0. A raw C array `int a[n]`
  (or `int a[n];` as a local) does NOT — garbage values. Prefer vector.
- Passing large vectors by value copies them — use `const vector<int>&`
  for read-only function params, `vector<int>&` if you need to modify.
- `*min_element(...)` / `*max_element(...)` — don't forget the `*`,
  these return iterators.

## Strings
- `s.find(x)` returns `string::npos` on failure, not `-1`. Compare to
  `string::npos`, never to `-1`.
- `s.substr(start, len)` — second argument is LENGTH, not an end index.
  Mixing this up with Python-style slicing is a common source of bugs
  for people coming from Python.
- Strings ARE mutable in C++ (`s[0] = 'x'` works) — unlike Java/Python
  where strings are immutable. Don't over-engineer workarounds for this.

## Pairs / Tuples
- `pair`/`tuple` compare lexicographically by default — use this to
  your advantage when sorting by (primary key, tie-break).
- `unordered_map<pair<int,int>, T>` does NOT compile out of the box —
  no default hash for pair. Use `map<pair<int,int>, T>` instead, or
  encode the pair as a single integer key.

## Maps
- `myMap[key]` on a NON-EXISTENT key SILENTLY INSERTS it (with a
  default value). If you just want to check existence, use
  `.find(key) != .end()` or `.count(key)`.
- `map` keeps keys sorted (O(log n) ops); `unordered_map` doesn't
  (O(1) average, but can be adversarially hacked to O(n) on CF).
- For small fixed key ranges (26 letters, 10 digits, etc.), a plain
  array (`int freq[26]`) beats any map in speed and simplicity.

## 2D Grids
- `grid[row][col]`, not `grid[col][row]` — pick a convention and never
  swap mid-problem. Bugs from this often only show up on non-square grids.
- Always bounds-check BEFORE indexing a neighbor cell, not after.
- `grid[0].size()` on a possibly-empty grid crashes — check `rows > 0`
  first if the grid could be empty.
- Mark cells visited when you PUSH them to the BFS queue, not only when
  you POP them — otherwise the same cell can be enqueued multiple times.

## General / Cross-cutting
- Any sum of up to `n` values, each up to `1e9`, with `n` up to `1e5+`,
  can overflow `int` (~2.1e9 max). Use `long long` for the accumulator
  even if individual elements fit in `int`.
- Reset any global/static array, vector, or map BETWEEN test cases when
  the problem has multiple test cases (`while(t--)`) — leftover state
  from test case 1 silently corrupting test case 2 is one of the most
  common WA causes for beginners.
- `endl` flushes the output buffer every call — use `'\n'` in loops
  with lots of output, or you risk TLE purely from I/O overhead.

## The debugging habit that actually moves your rating

When you get WA and can't see why: pick the SMALLEST input that could
possibly trigger your bug (often n=1 or n=2), trace your code by hand
line by line against that input, and compare to what you expect. Most
of the bugs above show up clearly the moment you trace a tiny case —
they hide in n=50,000 but not in n=2.
