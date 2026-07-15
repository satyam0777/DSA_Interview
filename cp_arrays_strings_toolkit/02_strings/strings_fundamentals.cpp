/*
    ============================================================
    STRINGS — std::string as a mutable, dynamically-sized char array.
    ============================================================
    Concept: std::string is basically vector<char> with extra convenience
    methods. Almost every array trick applies to strings too.
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

void basics() {
    string s = "hello world";

    // ---- indexing: O(1), just like an array ----
    char c = s[0]; // 'h'

    // ---- length ----
    int n = (int)s.size(); // .size() == .length(), same thing. Cast to int
                            // for the same unsigned-underflow reason as vectors.

    // ---- concatenation ----
    string t = s + "!";        // creates a new string, O(n)
    s += "!";                   // appends in place, amortized O(1) like push_back

    // ---- substring: s.substr(start, length) ----
    string sub = s.substr(0, 5);  // "hello" -- start index 0, length 5
    string tail = s.substr(6);     // from index 6 to the end (length omitted)
    // NOTE: substr COPIES -- O(length) each call. Calling it in a hot loop
    // on large strings is a common hidden-O(n^2) bug.

    // ---- find ----
    size_t pos = s.find("world"); // returns index of first match, or the
                                     // special value string::npos if not found
    if (pos != string::npos) {
        // found it at index `pos`
    }

    // ---- comparison ----
    // strings compare lexicographically with <, ==, etc. -- exactly like
    // you'd expect from dictionary order. Handy for sorting words.
    bool isLess = (string("abc") < string("abd")); // true

    (void)c; (void)n; (void)t; (void)sub; (void)tail; (void)isLess;
}

// ---------------------------------------------------------------
// CHAR <-> INT / DIGIT CONVERSIONS -- comes up constantly
// ---------------------------------------------------------------
void charConversions() {
    char c = '7';

    // char digit -> int value: subtract '0' (works because digits are
    // contiguous in ASCII: '0'=48, '1'=49, ..., '9'=57)
    int digit = c - '0'; // 7

    // int -> char digit: add '0' back
    char back = '0' + digit; // '7'

    // lowercase <-> uppercase: letters are also contiguous per case
    char lower = 'A' + 32;      // quick trick, but prefer tolower/toupper:
    char lo = tolower('A');     // 'a' -- handles edge cases correctly
    char up = toupper('a');     // 'A'

    // check character category
    bool isDigit = isdigit(c);
    bool isAlpha = isalpha(c);
    bool isSpace = isspace(c);

    (void)digit; (void)back; (void)lower; (void)lo; (void)up;
    (void)isDigit; (void)isAlpha; (void)isSpace;
}

// ---------------------------------------------------------------
// BUILDING / PARSING STRINGS
// ---------------------------------------------------------------
void buildingAndParsing() {
    // ---- int <-> string ----
    int x = 42;
    string s = to_string(x);       // 42 -> "42"
    int y = stoi("42");             // "42" -> 42
    ll z = stoll("123456789012");   // for values too big for int

    // ---- splitting a string by a delimiter (no built-in split in C++,
    // this is THE standard pattern using stringstream) ----
    string csv = "10,20,30,40";
    stringstream ss(csv);
    string token;
    vector<int> nums;
    while (getline(ss, token, ',')) {
        nums.push_back(stoi(token));
    }

    // ---- splitting by whitespace (simplest case: just use >>) ----
    string line = "10 20 30 40";
    stringstream ss2(line);
    vector<int> nums2;
    int val;
    while (ss2 >> val) nums2.push_back(val);

    // ---- joining strings with a separator ----
    vector<string> words = {"a", "b", "c"};
    string joined;
    for (int i = 0; i < (int)words.size(); ++i) {
        joined += words[i];
        if (i + 1 < (int)words.size()) joined += ",";
    }

    (void)s; (void)y; (void)z; (void)nums; (void)nums2; (void)joined;
}

// ---------------------------------------------------------------
// MUTATING STRINGS -- strings ARE mutable in C++ (unlike Java/Python)
// ---------------------------------------------------------------
void mutation() {
    string s = "hello";
    s[0] = 'H';              // direct index assignment works fine
    reverse(s.begin(), s.end());   // in-place reverse
    sort(s.begin(), s.end());       // in-place sort of characters

    // build a string character by character -- prefer this over
    // repeated `s += c` in EXTREMELY hot loops if you also know the
    // final size ahead of time (reserve avoids reallocations):
    string built;
    built.reserve(1000); // optional but avoids repeated reallocation
    for (int i = 0; i < 10; ++i) built += ('a' + i);
}

/*
    QUICK REFERENCE
    ----------------------------------------------------------
    s.size() / s.length()      length, O(1)
    s[i]                        index, O(1)
    s.substr(a, len)            substring copy, O(len)
    s.find(sub)                  first occurrence, O(n*m) worst case
                                  (use KMP/hashing for large-scale search --
                                  see 07_string_algorithms)
    s + t / s += t               concatenation
    to_string(x) / stoi(s)       int <-> string
    stringstream                 splitting/parsing tokens
    reverse(s.begin(), s.end())  in-place reverse
    sort(s.begin(), s.end())     in-place character sort (e.g. anagram check:
                                  sort both strings, compare equality)
    ----------------------------------------------------------
    PITFALL: s.find(x) returns string::npos (a huge unsigned value) on
    failure, NOT -1. Comparing `if (s.find(x) == -1)` is a classic bug --
    always compare to string::npos.
*/

int main() { return 0; }
