/*
    ============================================================
    PAIRS, TUPLES, AND VECTORS OF PAIRS
    ============================================================
    Concept: a `pair` bundles two related values into one object without
    writing a whole struct. This shows up EVERYWHERE: (value, index),
    (row, col), (distance, node), (start, end) of an interval, edges
    (u, v) or (u, v, weight) in a graph, and as the natural key/value
    unit when you sort or heap things by a combination of two fields.
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

void pairBasics() {
    pii p = {3, 7};          // construct with braces
    pii q = make_pair(3, 7); // equivalent, older style

    // ---- access ----
    int a = p.first;
    int b = p.second;

    // ---- comparison: pairs compare LEXICOGRAPHICALLY by default ----
    // compares .first first; if equal, compares .second. This is why
    // pair is so useful for sorting by (primary key, tie-break key)
    // with ZERO extra code -- just sort a vector<pair<...>> directly.
    bool less = (pii{1, 5} < pii{1, 6}); // true (first equal, second 5<6)

    // ---- structured bindings (C++17) -- unpack a pair cleanly ----
    auto [x, y] = p; // x = 3, y = 7, much cleaner than p.first/p.second
                       // everywhere in a long function

    (void)q; (void)a; (void)b; (void)less; (void)x; (void)y;
}

// ---------------------------------------------------------------
// VECTOR OF PAIRS -- the workhorse pattern
// ---------------------------------------------------------------
void vectorOfPairsPatterns() {
    // Pattern: "value with its original index" -- extremely common when
    // you need to sort but still remember where each element came from.
    vector<int> a = {50, 10, 40, 20};
    vector<pii> indexed; // {value, original_index}
    for (int i = 0; i < (int)a.size(); ++i) indexed.push_back({a[i], i});

    sort(indexed.begin(), indexed.end()); // sorts by value first (pair
                                            // default comparison), tie-break
                                            // by index automatically

    // now indexed[0] = smallest value with its original index -- you've
    // effectively sorted `a` while keeping track of where each element
    // was originally. This is THE standard way to answer "process
    // elements smallest to largest, but still know the original index."

    // Pattern: representing INTERVALS as pairs {start, end}, then
    // sorting by start (or by end, depending on the problem -- e.g.
    // classic "max non-overlapping intervals" sorts by END).
    vector<pii> intervals = {{1, 5}, {2, 3}, {4, 8}};
    sort(intervals.begin(), intervals.end()); // sorts by start by default

    // Pattern: representing graph EDGES as pairs (or tuples with weight).
    vector<pii> edges = {{0, 1}, {1, 2}, {2, 0}};

    // Pattern: representing GRID CELLS as {row, col} -- see 04_grids_2d
    // for the full traversal pattern built on this.
    pii cell = {2, 3}; // row 2, col 3

    (void)cell; (void)edges;
}

// ---------------------------------------------------------------
// TUPLE -- like pair but for 3+ values (e.g. weighted edges: u, v, w)
// ---------------------------------------------------------------
void tupleBasics() {
    tuple<int,int,int> edge = {0, 1, 10}; // u=0, v=1, weight=10

    // access with get<index>()
    int u = get<0>(edge);
    int v = get<1>(edge);
    int w = get<2>(edge);

    // structured bindings work here too -- much more readable
    auto [uu, vv, ww] = edge;

    // tuples also compare lexicographically, so you can sort a
    // vector<tuple<int,int,int>> of edges by weight by putting weight
    // FIRST in the tuple (classic trick for Kruskal's MST):
    vector<tuple<int,int,int>> weightedEdges = {{5, 0, 1}, {2, 1, 2}}; // {w, u, v}
    sort(weightedEdges.begin(), weightedEdges.end()); // sorts by w ascending

    (void)u; (void)v; (void)w; (void)uu; (void)vv; (void)ww;
}

// ---------------------------------------------------------------
// PAIR AS A MAP KEY -- for 2D state lookups (memoization, visited sets)
// ---------------------------------------------------------------
void pairAsMapKey() {
    // map<pair<int,int>, ...> works out of the box because pair already
    // has operator< defined (needed for the ordered `map`).
    map<pii, int> memo; // e.g. memo[{row, col}] = best answer from here
    memo[{0, 0}] = 5;

    // unordered_map<pair<int,int>, ...> does NOT work out of the box --
    // pair has no default std::hash specialization. Either:
    // (a) use ordered `map` (slightly slower, O(log n), but zero setup), or
    // (b) encode the pair as a single integer key, e.g. for coordinates
    //     bounded by [0, 10^5): key = row * 100000 + col — this is the
    //     standard CP trick to avoid writing a custom hash function.
    int row = 3, col = 7, COLS = 100000;
    ll key = (ll)row * COLS + col;
    unordered_map<ll, int> fastMemo;
    fastMemo[key] = 5;

    (void)memo;
}

/*
    QUICK REFERENCE -- "what container for this state?"
    ----------------------------------------------------------
    (value, original index)              -> vector<pair<int,int>>
    (row, col) on a grid                  -> pair<int,int>, or two ints
    (start, end) of an interval           -> pair<int,int>
    (distance, node) for Dijkstra's heap  -> pair<ll,int>
    (u, v, weight) for an edge             -> tuple<int,int,int>
                                              (put weight FIRST if you'll
                                              sort edges by weight)
    2D memoization key                     -> map<pair<int,int>,T> (simple)
                                              or encode as single ll key
                                              into unordered_map (faster)
    ----------------------------------------------------------
*/

int main() { return 0; }
