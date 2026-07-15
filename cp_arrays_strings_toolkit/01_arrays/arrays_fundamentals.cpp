/*
    ============================================================
    ARRAYS — C-style array vs std::vector, and the operations you'll
    actually use in a contest.
    ============================================================
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

void staticVsDynamic() {
    // ---- C-style array: fixed size, known at compile time ----
    // Fine for small, fixed bounds (e.g. "at most 26 letters",
    // "at most 4 directions"). NOT good when size depends on input n,
    // because n could be large and you'd be guessing an upper bound.
    int fixedArr[26] = {0}; // {0} zero-initializes ALL elements -- without
                             // this, a local array has GARBAGE values.
                             // Classic silent bug: forgetting to init.

    // ---- vector: dynamic size, the CP default for "array of size n" ----
    int n = 5;
    vector<int> a(n, 0);       // size n, all initialized to 0
    vector<int> b(n);          // size n, ints default to 0 anyway (unlike
                                // raw arrays, vector<int> value-initializes)
    vector<vector<int>> grid(3, vector<int>(4, -1)); // 3x4 grid, all -1

    (void)fixedArr; (void)a; (void)b; (void)grid;
}

void basicOperations() {
    vector<int> a = {5, 3, 8, 1, 9};

    // ---- size ----
    int n = a.size(); // .size() returns size_t (unsigned!). Comparing an
                       // unsigned size() to a negative int, or subtracting
                       // past 0, causes silent bugs (wraps to a huge
                       // positive number). Cast to int when in doubt:
    int sz = (int)a.size();

    // ---- append / remove from the end: O(1) amortized ----
    a.push_back(100);
    a.pop_back();

    // ---- insert/erase in the middle: O(n), avoid in hot loops ----
    a.insert(a.begin() + 2, 42);  // insert 42 at index 2, shifts rest right
    a.erase(a.begin() + 2);        // remove index 2, shifts rest left

    // ---- sort ----
    sort(a.begin(), a.end());              // ascending
    sort(a.rbegin(), a.rend());            // descending (reverse iterators)

    // ---- reverse ----
    reverse(a.begin(), a.end());

    // ---- min/max element ----
    int mn = *min_element(a.begin(), a.end());
    int mx = *max_element(a.begin(), a.end());
    // NOTE the dereference `*` -- min_element returns an ITERATOR, not
    // the value. Forgetting `*` is a classic beginner compile error.

    // ---- sum ----
    ll total = accumulate(a.begin(), a.end(), 0LL); // 0LL, not 0, to avoid
                                                        // int overflow on sum

    // ---- fill ----
    fill(a.begin(), a.end(), 0); // set every element to 0

    // ---- resize (grow or shrink) ----
    a.resize(10, -1); // grow to size 10, new elements = -1 (shrinking just
                        // truncates, the -1 is ignored in that case)

    (void)n; (void)sz; (void)mn; (void)mx; (void)total;
}

// ---------------------------------------------------------------
// PASSING ARRAYS/VECTORS TO FUNCTIONS
// ---------------------------------------------------------------
// Passing by VALUE copies the whole vector -- O(n) copy, wasteful and
// slow for large n, and any modification inside the function does NOT
// affect the caller's vector.
void byValue(vector<int> v) { v[0] = 999; } // caller's vector unaffected

// Passing by REFERENCE avoids the copy AND lets you modify the caller's
// data. Use `const vector<int>&` if you only need to READ it (still
// avoids the copy, and the const signals "I won't modify this" both to
// the compiler and to future-you reading the code).
void byReference(vector<int>& v) { v[0] = 999; }        // can modify
void byConstReference(const vector<int>& v) {             // read-only
    int x = v[0];
    (void)x;
}

// ---------------------------------------------------------------
// CLASSIC PITFALLS
// ---------------------------------------------------------------
void pitfalls() {
    vector<int> a = {1, 2, 3};

    // 1. Off-by-one: a valid index range is [0, a.size()-1]. a[a.size()]
    //    is out of bounds -- vector does NOT check this by default with
    //    operator[] (undefined behavior, might not even crash locally,
    //    then crashes or gives wrong answer on the judge). Use a.at(i)
    //    while debugging locally -- it throws on out-of-bounds, at the
    //    cost of being slower, so swap back to operator[] before final
    //    submission if speed matters.

    // 2. Unsigned size() underflow:
    //    for (int i = 0; i < a.size() - 1; i++) -- if a.size() is 0,
    //    a.size()-1 UNDERFLOWS to a huge unsigned number (since size()
    //    is size_t), and the loop runs "forever" (until it crashes).
    //    Fix: cast to int first, or check a.size() >= 1 before subtracting.
    int n = (int)a.size();
    if (n >= 1) {
        for (int i = 0; i < n - 1; i++) { /* safe now */ }
    }

    // 3. Modifying a vector while iterating with a range-for can
    //    invalidate iterators (e.g. push_back inside a range-for over
    //    the same vector may reallocate memory and crash). Don't
    //    resize/push/pop a container while range-for'ing over it.

    // 4. 2D vector aliasing bug: vector<vector<int>> grid(n, vector<int>(m))
    //    is fine (each row is a genuinely separate vector). But
    //    vector<int>* rows = new vector<int>[n]; then trying to share
    //    one row's memory across rows is a classic pointer mistake --
    //    just don't hand-roll 2D arrays with raw pointers in CP, use
    //    vector<vector<int>>.
}

int main() { return 0; }
