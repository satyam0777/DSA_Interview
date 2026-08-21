/*
    ============================================================
    STRING ALGORITHMS -- the "advanced" toolbox, once basics feel easy
    ============================================================
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// ---------------------------------------------------------------
// FREQUENCY COUNTING -- the most-used string trick by far.
// For lowercase English letters, a fixed-size array beats a map: O(1)
// access with no hashing overhead, and only 26 ints of memory.
// ---------------------------------------------------------------
bool isAnagram(string& s1, string& s2) {
    if (s1.size() != s2.size()) return false;
    int freq[26] = {0}; // zero-initialized
    for (char c : s1) freq[c - 'a']++;
    for (char c : s2) freq[c - 'a']--;
    for (int i = 0; i < 26; ++i) if (freq[i] != 0) return false;
    return true;
    // ALTERNATE approach: sort both strings and compare equality.
    // O(n log n) instead of O(n), but a one-liner -- fine unless n is huge.
}

// ---------------------------------------------------------------
// PALINDROME CHECK -- two pointers from both ends, O(n)
// ---------------------------------------------------------------
bool isPalindrome(string& s) {
    int l = 0, r = (int)s.size() - 1;
    while (l < r) {
        if (s[l] != s[r]) return false;
        l++; r--;
    }
    return true;
}

// ---------------------------------------------------------------
// POLYNOMIAL ROLLING HASH -- turns a string (or substring) into a
// single number, so you can compare substrings for equality in O(1)
// after O(n) preprocessing, instead of O(length) per comparison.
// Concept: treat the string as a number in some base B, mod a large
// prime, i.e. hash(s) = s[0]*B^(n-1) + s[1]*B^(n-2) + ... + s[n-1] (mod p).
// Prefix hashes let you extract the hash of ANY substring in O(1).
// USE CASE: fast substring comparison, finding duplicate substrings,
// string matching, avoiding O(n*m) brute-force comparisons.
// ---------------------------------------------------------------
struct RollingHash {
    vector<ll> prefixHash, powB;
    ll base, mod;

    RollingHash(const string& s, ll baseArg = 131, ll modArg = 1e9 + 7311)
        : base(baseArg), mod(modArg) {
        int n = s.size();
        prefixHash.assign(n + 1, 0);
        powB.assign(n + 1, 1);
        for (int i = 0; i < n; ++i) {
            prefixHash[i+1] = (prefixHash[i] * base + s[i]) % mod;
            powB[i+1] = (powB[i] * base) % mod;
        }
    }

    // hash of s[l..r] inclusive, 0-indexed
    ll getHash(int l, int r) {
        ll result = (prefixHash[r+1] - prefixHash[l] * powB[r-l+1]) % mod;
        if (result < 0) result += mod; // C++ % can return negative for
                                          // negative operands -- always
                                          // normalize back into [0, mod)
        return result;
    }
};

void rollingHashUsage() {
    string s = "abcabcabc";
    RollingHash rh(s);
    bool sameSubstring = (rh.getHash(0, 2) == rh.getHash(3, 5)); // "abc" == "abc" -> true
    (void)sameSubstring;
    // NOTE: hash collisions are theoretically possible (two different
    // substrings, same hash) -- for contest-safety against adversarial
    // hacks, some setters use DOUBLE hashing (two different mod/base
    // pairs, both must match) or a large random mod chosen at runtime.
}

// ---------------------------------------------------------------
// KMP PREFIX FUNCTION -- pi[i] = length of the longest proper prefix
// of s[0..i] that is ALSO a suffix of s[0..i]. Powers substring search
// in O(n+m) instead of the naive O(n*m), and solves a family of
// "string periodicity" problems.
// ---------------------------------------------------------------
vector<int> prefixFunction(const string& s) {
    int n = s.size();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; ++i) {
        int j = pi[i-1];
        // fall back through shorter candidate prefixes while they
        // don't extend -- this amortizes to O(n) total across the loop
        while (j > 0 && s[i] != s[j]) j = pi[j-1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

// find all occurrences of `pattern` in `text`, O(n + m)
vector<int> kmpSearch(const string& text, const string& pattern) {
    string combined = pattern + "#" + text; // '#' separator must not
                                              // appear in either string
    vector<int> pi = prefixFunction(combined);
    vector<int> occurrences;
    int patLen = pattern.size();
    for (int i = patLen + 1; i < (int)combined.size(); ++i) {
        if (pi[i] == patLen) {
            occurrences.push_back(i - 2 * patLen); // start index in `text`
        }
    }
    return occurrences;
}

/*
    QUICK REFERENCE -- "what string tool do I need?"
    ----------------------------------------------------------
    "count/compare character frequencies"        -> freq[26] array
    "check palindrome"                             -> two pointers, O(n)
    "compare many substrings for equality fast"    -> rolling hash
    "find all occurrences of a pattern in a text"  -> KMP (or built-in
                                                       s.find in a loop
                                                       if n, m are small)
    "longest common substring/subsequence"          -> DP (see section 06)
    ----------------------------------------------------------
    A NOTE ON WHEN TO REACH FOR THESE: on Codeforces, plenty of Div2 A/B
    problems only need frequency counting and two pointers. Rolling hash
    and KMP tend to show up from Div2 D/E onward, or in dedicated "string"
    tagged problems. Don't feel like you need these on day one -- master
    01-06 first, they cover the large majority of problems you'll see
    early on.
*/

int main() { return 0; }
