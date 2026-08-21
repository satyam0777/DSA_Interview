/*
    ============================================================
    MAPS, STRUCTS, AND "OBJECTS" IN CP
    ============================================================
    Concept: CP rarely needs full OOP (inheritance, polymorphism). What
    you DO need constantly: a way to bundle related fields together
    (struct), and a way to look values up by a key (map/unordered_map).
    This file also covers combining them: map of vectors, map of pairs,
    vector of structs -- the shapes that come up in real problems.
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// ---------------------------------------------------------------
// MAP vs UNORDERED_MAP
// ---------------------------------------------------------------
void mapBasics() {
    // map: RED-BLACK TREE internally -- keys stay SORTED, O(log n) per
    // operation. Use when you need sorted iteration, or range queries
    // (lower_bound/upper_bound work on map too!).
    map<string, int> wordCount;
    wordCount["apple"]++;   // if "apple" doesn't exist yet, map DEFAULT-
                              // CONSTRUCTS it (0 for int), THEN increments
                              // -- this "insert on first access" behavior
                              // is extremely convenient for counting.
    wordCount["apple"]++;

    // unordered_map: HASH TABLE internally -- no ordering guarantee,
    // O(1) average per operation (but O(n) worst case, and CF problems
    // can sometimes be adversarially hacked against naive unordered_map
    // -- if that's a concern, use map, or a custom/stronger hash).
    unordered_map<string, int> fastCount;
    fastCount["banana"]++;

    // ---- checking existence WITHOUT inserting ----
    // wordCount["missing"] would SILENTLY INSERT "missing" with value 0
    // as a side effect of just checking -- classic subtle bug if you
    // then iterate the map expecting only keys you explicitly added.
    // Use .find() or .count() to check without inserting:
    if (wordCount.find("apple") != wordCount.end()) { /* exists */ }
    if (wordCount.count("apple")) { /* exists, count() is 0 or 1 for map */ }

    // ---- iterating a map (sorted order by key, for `map` not `unordered_map`) ----
    for (auto& [word, cnt] : wordCount) {
        // word, cnt available here, in ASCENDING key order for `map`
        (void)word; (void)cnt;
    }
}

// ---------------------------------------------------------------
// MAP OF VECTORS -- "group items by a key" (adjacency lists ARE this!)
// ---------------------------------------------------------------
void mapOfVectors() {
    // e.g. group students by grade, or build a graph adjacency list
    // when node labels aren't small contiguous ints (so you can't just
    // use vector<vector<int>> adj(n) directly).
    map<string, vector<int>> groups;
    groups["A"].push_back(90);   // auto-creates the vector on first use
    groups["A"].push_back(85);
    groups["B"].push_back(70);

    // classic adjacency list when nodes are strings/labels, not 0..n-1 ints:
    map<string, vector<string>> graph;
    graph["Alice"].push_back("Bob");   // Alice -> Bob edge
}

// ---------------------------------------------------------------
// MAP OF PAIRS / PAIR AS VALUE -- store two related pieces of info per key
// ---------------------------------------------------------------
void mapOfPairs() {
    // e.g. for each value, remember {count, last_index_seen}
    map<int, pair<int,int>> info; // value -> {count, last_index}
    vector<int> a = {5, 3, 5, 5, 3};
    for (int i = 0; i < (int)a.size(); ++i) {
        auto& [cnt, lastIdx] = info[a[i]]; // reference into the map --
                                              // modifying cnt/lastIdx here
                                              // modifies the stored pair directly
        cnt++;
        lastIdx = i;
    }
}

// ---------------------------------------------------------------
// STRUCTS -- when a pair/tuple isn't descriptive enough (3+ named fields,
// or you want member functions / a custom comparator attached directly)
// ---------------------------------------------------------------
struct Student {
    string name;
    int score;
    int age;

    // defining operator< lets you sort/use in set/map/priority_queue
    // WITHOUT writing a separate comparator every time you need one.
    bool operator<(const Student& other) const {
        if (score != other.score) return score > other.score; // higher
                                                                  // score
                                                                  // first
        return name < other.name; // tie-break alphabetically
    }
};

void structUsage() {
    vector<Student> students = {
        {"Alice", 90, 20},
        {"Bob", 85, 21},
        {"Carol", 90, 19}
    };
    sort(students.begin(), students.end()); // uses Student::operator<

    // WHY A STRUCT HERE INSTEAD OF A TUPLE<STRING,INT,INT>: `student.score`
    // is self-documenting. `get<1>(student)` is not -- you'd have to
    // remember field order. Use struct once you have 3+ fields or the
    // code will be read/modified many times; a pair/tuple is fine for
    // quick, 2-3-field throwaway use.
}

// ---------------------------------------------------------------
// WHEN TO USE WHAT -- pair vs tuple vs struct vs map
// ---------------------------------------------------------------
/*
    2 related values, short-lived, order is obvious       -> pair
    3+ related values, short-lived, order remembered easily -> tuple
    3+ related values, OR reused across many functions,
      OR needs a custom comparator/methods attached          -> struct
    "look up value(s) by a key, key space is sparse/large"   -> map/unordered_map
    "group multiple values under one key"                    -> map<K, vector<V>>
    "small FIXED set of keys (e.g. 26 letters, 10 digits)"    -> plain array,
                                                                  freq[26] or
                                                                  freq[10] --
                                                                  faster than
                                                                  any map for
                                                                  this case
*/

int main() { return 0; }
