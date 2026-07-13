/*
 * COMPARATOR FUNCTIONS
 * ----------------------
 * This is the #1 source of "I know the logic but couldn't implement it" bugs
 * on Codeforces. Custom sort orders show up in probably 1/3 of all problems.
 *
 * THE GOLDEN RULE: a comparator `cmp(a, b)` must return TRUE if "a should come
 * BEFORE b" in the final order. It must define a STRICT WEAK ORDERING:
 *   - cmp(a, a) must be FALSE (never say an element comes before itself)
 *   - if cmp(a,b) is true, cmp(b,a) must be false (no contradictions)
 * Getting this wrong causes undefined behavior / crashes with std::sort on
 * large inputs (it won't always show up on small test cases - a classic CF trap).
 *
 * Covered:
 *   1. Sorting pairs/vectors by a specific field (not the default lexicographic order)
 *   2. Sorting a struct with a custom comparator (both lambda and operator< versions)
 *   3. Custom comparator for priority_queue (min-heap of a struct)
 *   4. Sorting indices instead of the array itself (very common CF pattern)
 *   5. Multi-criteria sort (sort by field A, break ties with field B)
 *
 * Compile: g++ -std=c++17 -O2 -o cmp comparators.cpp && ./cmp
 */

#include <bits/stdc++.h>
using namespace std;

struct Person {
    string name;
    int age;
    int score;
};

int main() {
    // ---------------- 1) Sort pairs by SECOND element (default sorts by first, then second) ----------------
    vector<pair<int,int>> pairs = {{1, 5}, {2, 3}, {1, 2}, {3, 1}};
    sort(pairs.begin(), pairs.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
        return a.second < b.second;  // ascending by second element
    });
    cout << "Sorted by second element: ";
    for (auto& p : pairs) cout << "(" << p.first << "," << p.second << ") ";
    cout << "(expected (3,1)(1,2)(2,3)(1,5))\n\n";

    // ---------------- 2) Sort a struct: lambda comparator vs operator< ----------------
    vector<Person> people = {
        {"Alice", 30, 85},
        {"Bob", 25, 90},
        {"Carol", 25, 70}
    };
    // Using a lambda - most common in contest code, no need to touch the struct
    sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.age < b.age; // ascending age
    });
    cout << "Sorted by age: ";
    for (auto& p : people) cout << p.name << "(" << p.age << ") ";
    cout << "\n\n";

    // ---------------- 3) Custom comparator for priority_queue ----------------
    // priority_queue is a MAX-heap by default using operator<. To make a min-heap
    // of a custom struct by `score`, provide a comparator that returns true when
    // "a has LOWER priority than b" (i.e. reversed compared to normal sort logic!).
    // CAUTION: priority_queue comparator semantics are the OPPOSITE of sort's -
    // "return true" means "a should come out LATER", not "a should come first".
    auto cmp = [](const Person& a, const Person& b) {
        return a.score > b.score; // this makes it a MIN-heap by score (lowest score on top)
    };
    priority_queue<Person, vector<Person>, decltype(cmp)> pq(cmp);
    for (auto& p : people) pq.push(p);

    cout << "Priority queue (min-heap by score) pop order: ";
    while (!pq.empty()) {
        cout << pq.top().name << "(" << pq.top().score << ") ";
        pq.pop();
    }
    cout << "(expected Carol(70) Alice(85) Bob(90))\n\n";

    // ---------------- 4) Sort INDICES instead of the array (extremely common CF pattern) ----------------
    // Useful when you need the sorted ORDER but must still refer back to original positions/other arrays.
    vector<int> values = {40, 10, 30, 20};
    vector<int> idx(values.size());
    iota(idx.begin(), idx.end(), 0); // idx = {0, 1, 2, 3}

    sort(idx.begin(), idx.end(), [&](int i, int j) {
        return values[i] < values[j]; // compare by the VALUE at each index, but sort the index array
    });
    cout << "Original values: ";
    for (int v : values) cout << v << " ";
    cout << "\nSorted index order: ";
    for (int i : idx) cout << i << " ";
    cout << " (expected 1 3 2 0, since values[1]=10 is smallest)\n\n";

    // ---------------- 5) Multi-criteria sort: sort by score DESC, break ties by age ASC ----------------
    vector<Person> people2 = {
        {"Dave", 22, 80},
        {"Eve", 20, 90},
        {"Frank", 25, 80}
    };
    sort(people2.begin(), people2.end(), [](const Person& a, const Person& b) {
        if (a.score != b.score) return a.score > b.score;  // primary: score descending
        return a.age < b.age;                                // tie-break: age ascending
    });
    cout << "Multi-criteria sort (score desc, age asc tiebreak): ";
    for (auto& p : people2) cout << p.name << "(score=" << p.score << ",age=" << p.age << ") ";
    cout << "\n(expected Eve(90,20) Dave(80,22) Frank(80,25))\n";

    return 0;
}

/*
 * COMMON MISTAKES THAT CAUSE RUNTIME CRASHES / WRONG ANSWERS ON LARGE INPUT:
 *   - Using <= instead of < in a comparator (violates strict weak ordering, can crash
 *     std::sort silently on large arrays - ALWAYS use strict < or > in comparators).
 *   - Forgetting `const &` in the comparator's parameters (works but does unnecessary copies -
 *     slow on structs with strings/vectors, can TLE on large n).
 *   - Mixing up the direction for priority_queue comparators (remember: opposite of intuition -
 *     `return a.x > b.x` gives a MIN-heap, not a max-heap, because pq pops the "largest" per cmp).
 *
 * TRY YOURSELF:
 *   - Sort a vector of vectors by the LAST element of each inner vector.
 *   - Implement a comparator for sorting strings by LENGTH first, then lexicographically.
 *   - CF problems tagged "sortings" on codeforces.com/problemset for practice.
 */
