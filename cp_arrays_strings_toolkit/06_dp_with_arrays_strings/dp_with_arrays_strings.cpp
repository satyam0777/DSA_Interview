/*
    ============================================================
    HOW ARRAYS/STRINGS BECOME DP -- the translation you asked about
    ============================================================
    Concept: DP is just "an array/grid whose index IS the state, and
    whose value is the answer for that state." Once you can write
    dp[i] = ... in words, translating to code is close to mechanical.
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll NEG_INF = LLONG_MIN / 2;

// ---------------------------------------------------------------
// 1D DP OVER AN ARRAY -- classic example: House Robber / max sum of
// non-adjacent elements.
// dp[i] = best answer considering the first i elements.
// ---------------------------------------------------------------
ll maxNonAdjacentSum(vector<int>& a) {
    int n = a.size();
    if (n == 0) return 0;
    vector<ll> dp(n + 1, 0);
    // dp[0] = 0 (no elements considered)
    // dp[1] = a[0] (only first element available)
    dp[1] = a[0];
    for (int i = 2; i <= n; ++i) {
        // at element i-1 (0-indexed a[i-1]): either SKIP it (dp[i-1]) or
        // TAKE it (a[i-1] + best answer from BEFORE the previous element,
        // i.e. dp[i-2], since we can't take two adjacent elements)
        dp[i] = max(dp[i-1], dp[i-2] + a[i-1]);
    }
    return dp[n];
    // NOTICE: dp is indexed 1..n while a is indexed 0..n-1 -- this
    // "offset by one" pattern (dp[i] represents "first i elements") is
    // extremely common and avoids negative-index headaches for dp[i-2]
    // when i is small.
}

// ---------------------------------------------------------------
// 2D DP OVER TWO STRINGS -- classic example: Longest Common Subsequence
// dp[i][j] = LCS length using first i chars of s1 and first j chars of s2.
// ---------------------------------------------------------------
int longestCommonSubsequence(string& s1, string& s2) {
    int n = s1.size(), m = s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s1[i-1] == s2[j-1]) {
                // characters match: extend the LCS found without these
                // two characters by 1
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                // characters don't match: best of "drop last char of s1"
                // or "drop last char of s2"
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }
    return dp[n][m];
    // SAME offset-by-one idea as before: dp[i][j] means "first i chars
    // of s1, first j chars of s2" so s1[i-1] is the i-th character
    // (0-indexed access, 1-indexed dp state).
}

// ---------------------------------------------------------------
// 2D DP OVER ONE STRING -- classic example: Longest Palindromic Substring
// dp[i][j] = true if s[i..j] (inclusive) is a palindrome.
// ---------------------------------------------------------------
string longestPalindromicSubstring(string& s) {
    int n = s.size();
    if (n == 0) return "";
    vector<vector<bool>> dp(n, vector<bool>(n, false));
    int bestLen = 1, bestStart = 0;

    // base cases: single characters are always palindromes
    for (int i = 0; i < n; ++i) dp[i][i] = true;

    // fill by INCREASING LENGTH -- dp[i][j] depends on dp[i+1][j-1]
    // (the substring one shorter on each side), so shorter lengths
    // must be computed first. This "iterate by length" order is a
    // recurring DP-over-strings pattern, easy to get backwards if you
    // just loop i then j naively.
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j]) {
                dp[i][j] = (len == 2) || dp[i+1][j-1];
            }
            if (dp[i][j] && len > bestLen) {
                bestLen = len;
                bestStart = i;
            }
        }
    }
    return s.substr(bestStart, bestLen);
}

// ---------------------------------------------------------------
// DP WITH A PAIR-SHAPED STATE -- when one index isn't enough.
// Example: "minimum cost path in a grid" -- state is (row, col), so
// the DP table is naturally a 2D array indexed exactly by that pair.
// This is why grids (section 04) and DP connect directly: the grid IS
// the DP table in many problems.
// ---------------------------------------------------------------
int minPathSum(vector<vector<int>>& grid) {
    int rows = grid.size(), cols = grid[0].size();
    vector<vector<int>> dp(rows, vector<int>(cols, 0));
    dp[0][0] = grid[0][0];
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (r == 0 && c == 0) continue;
            int fromTop = (r > 0) ? dp[r-1][c] : INT_MAX;
            int fromLeft = (c > 0) ? dp[r][c-1] : INT_MAX;
            dp[r][c] = grid[r][c] + min(fromTop, fromLeft);
        }
    }
    return dp[rows-1][cols-1];
}

// ---------------------------------------------------------------
// MEMOIZATION WITH A MAP -- when your state doesn't map cleanly to
// small contiguous array indices (e.g. state includes a large/sparse
// value), use map<StateType, Result> instead of a plain array. Slower
// per-access (O(log n) vs O(1)) but far simpler to write correctly.
// ---------------------------------------------------------------
map<pair<int,int>, ll> memo;
ll recursiveWithMapMemo(int i, int remainingCapacity, vector<int>& weights, vector<int>& values) {
    if (i == (int)weights.size()) return 0;
    auto key = make_pair(i, remainingCapacity);
    if (memo.count(key)) return memo[key]; // already solved this exact state

    ll skip = recursiveWithMapMemo(i + 1, remainingCapacity, weights, values);
    ll take = NEG_INF;
    if (weights[i] <= remainingCapacity) {
        take = values[i] + recursiveWithMapMemo(i + 1, remainingCapacity - weights[i], weights, values);
    }
    return memo[key] = max(skip, take); // store BEFORE returning, so
                                          // future calls with the same
                                          // key hit the cache
}

/*
    QUICK REFERENCE -- "what shape is my DP table?"
    ----------------------------------------------------------
    state = one index into an array               -> dp[i], vector<T>
    state = position in a 2D grid                   -> dp[r][c], vector<vector<T>>
                                                        (often literally reuses
                                                        the grid's own dimensions)
    state = (index in s1, index in s2)               -> dp[i][j], classic for
                                                        LCS / edit distance /
                                                        string matching between
                                                        TWO strings
    state = (index, remaining capacity/budget/bitmask) -> dp[i][state], or a
                                                          map if state is sparse
    state includes something with a huge/sparse range  -> map<StateTuple, T>
                                                          memoization instead
                                                          of a plain array
    ----------------------------------------------------------
    THE TRANSLATION RECIPE:
    1. In one sentence, describe what dp[state] MEANS (not how to compute
       it yet -- just what it represents).
    2. Figure out the base case(s) -- the smallest/simplest state(s) you
       can answer directly.
    3. Write the recurrence: how does dp[state] relate to smaller/earlier
       states? This is usually "try each choice, take the best."
    4. Decide the fill order (increasing i, increasing length, etc.) so
       that whenever you compute dp[state], everything it depends on is
       already computed.
    5. Only THEN turn it into a loop/array -- steps 1-4 are the actual
       "logic", step 5 is just implementation.
*/

int main() { return 0; }
