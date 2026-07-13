/*
 * MATH FOR COMPETITIVE PROGRAMMING
 * -----------------------------------
 * The recurring math toolkit for CF problems beyond basic modpow/sieve
 * (already covered in 05_number_theory/sieve_modpow.cpp). This file covers
 * what comes up next once you're past the basics.
 *
 * Covered:
 *   1. GCD / LCM (std::gcd exists in C++17, but know how it works)
 *   2. Precomputed factorials + inverse factorials -> nCr mod p in O(1) per query
 *   3. Euler's Totient function (phi) - count of numbers <= n coprime to n
 *   4. Matrix exponentiation - compute linear recurrences (like Fibonacci) in O(log n)
 *   5. Modular arithmetic gotchas (negative mod, overflow)
 *
 * Compile: g++ -std=c++17 -O2 -o math math_for_cp.cpp && ./math
 */

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD = 1e9 + 7;

// ---------------- 1) GCD / LCM ----------------
// std::gcd(a,b) and std::lcm(a,b) exist in <numeric> since C++17 - use them directly.
// Shown here manually so you understand what's happening under the hood.
ll gcdManual(ll a, ll b) {
    while (b) { a %= b; swap(a, b); }
    return a;
}
ll lcmManual(ll a, ll b) {
    return a / gcdManual(a, b) * b; // divide FIRST to avoid overflow before multiplying
}

// ---------------- 2) Factorials + inverse factorials -> O(1) nCr mod p ----------------
// nCr(n, r) = n! / (r! * (n-r)!)  -->  mod p:  fact[n] * invFact[r] * invFact[n-r] mod p
// Precompute once in O(n), then answer any nCr query in O(1). ESSENTIAL for combinatorics
// problems where you need many nCr queries (counting problems, "number of ways to...").
const int MAXN = 1000006;
vector<ll> fact, invFact;

ll power(ll a, ll b, ll m) {
    a %= m;
    ll result = 1;
    while (b > 0) {
        if (b & 1) result = (result * a) % m;
        a = (a * a) % m;
        b >>= 1;
    }
    return result;
}

void precomputeFactorials(int n) {
    fact.assign(n + 1, 1);
    invFact.assign(n + 1, 1);
    for (int i = 1; i <= n; i++) fact[i] = fact[i-1] * i % MOD;
    invFact[n] = power(fact[n], MOD - 2, MOD); // Fermat's little theorem (MOD must be prime)
    for (int i = n - 1; i >= 0; i--) invFact[i] = invFact[i+1] * (i + 1) % MOD;
}

ll nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

// ---------------- 3) Euler's Totient Function ----------------
// phi(n) = count of integers in [1, n] that are coprime to n.
// Computed via the prime factorization formula: phi(n) = n * PRODUCT(1 - 1/p) over
// each distinct prime factor p of n. Useful for problems about coprimality, and phi
// also appears in Euler's theorem generalization of Fermat's little theorem.
ll eulerPhi(ll n) {
    ll result = n;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p; // multiply by (1 - 1/p), rearranged to avoid fractions
        }
    }
    if (n > 1) result -= result / n; // n itself is a remaining prime factor > sqrt(original n)
    return result;
}

// ---------------- 4) Matrix Exponentiation - solve linear recurrences in O(log n) ----------------
// Any linear recurrence (like Fibonacci: f(n) = f(n-1) + f(n-2)) can be expressed as
// matrix multiplication, then computed with fast exponentiation - crucial when n is
// huge (up to 10^18) and you can't just loop.
typedef vector<vector<ll>> Matrix;

Matrix matMul(const Matrix& a, const Matrix& b, ll mod) {
    int n = a.size(), m = b[0].size(), k = b.size();
    Matrix result(n, vector<ll>(m, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            for (int l = 0; l < k; l++)
                result[i][j] = (result[i][j] + a[i][l] * b[l][j]) % mod;
    return result;
}

Matrix matPow(Matrix base, ll exp, ll mod) {
    int n = base.size();
    Matrix result(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1; // identity matrix (multiplicative identity)
    while (exp > 0) {
        if (exp & 1) result = matMul(result, base, mod);
        base = matMul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// nth Fibonacci number mod p, computed via matrix exponentiation in O(log n)
// [F(n+1)]   [1 1]^n   [F(1)]
// [F(n)  ] = [1 0]   * [F(0)]
ll fibonacciMatrix(ll n, ll mod) {
    if (n == 0) return 0;
    Matrix base = {{1, 1}, {1, 0}};
    Matrix result = matPow(base, n - 1, mod);
    return result[0][0]; // this equals F(n)
}

int main() {
    // Test 1: GCD/LCM
    cout << "=== GCD / LCM ===\n";
    cout << "gcdManual(48, 18): " << gcdManual(48, 18) << " (expected 6)\n";
    cout << "lcmManual(4, 6): " << lcmManual(4, 6) << " (expected 12)\n";
    cout << "std::gcd(48, 18): " << gcd(48, 18) << " (built-in, same answer)\n\n";

    // Test 2: nCr
    cout << "=== nCr mod p ===\n";
    precomputeFactorials(1000);
    cout << "nCr(10, 3): " << nCr(10, 3) << " (expected 120)\n";
    cout << "nCr(50, 25): " << nCr(50, 25) << " (large value, computed mod 1e9+7)\n\n";

    // Test 3: Euler's Totient
    cout << "=== Euler's Totient ===\n";
    for (ll n : {9, 10, 13}) {
        cout << "phi(" << n << "): " << eulerPhi(n) << "\n";
    }
    cout << "(expected phi(9)=6, phi(10)=4, phi(13)=12 since 13 is prime)\n\n";

    // Test 4: Matrix exponentiation for Fibonacci
    cout << "=== Fibonacci via Matrix Exponentiation ===\n";
    for (ll n : {1, 2, 3, 10, 50}) {
        cout << "F(" << n << ") mod 1e9+7 = " << fibonacciMatrix(n, MOD) << "\n";
    }
    cout << "(expected F(1)=1 F(2)=1 F(3)=2 F(10)=55 F(50)=586268941 [i.e. 12586269025 mod 1e9+7])\n\n";

    // Test 5: modular arithmetic gotcha - negative mod
    cout << "=== Modular Arithmetic Gotcha ===\n";
    ll x = -7, m = 3;
    cout << "In C++, -7 % 3 = " << (x % m) << " (NOT the mathematical mod! C++ % can be negative)\n";
    cout << "Correct positive mod: ((x % m) + m) % m = " << (((x % m) + m) % m) << " (expected 2)\n";
    cout << "ALWAYS use ((x % m) + m) % m when x might be negative, e.g. after subtraction.\n";

    return 0;
}

/*
 * TRY YOURSELF:
 *   - Use nCr to solve "count paths in a grid" (LeetCode 62 has a formula solution: C(m+n-2, m-1)).
 *   - Extend matrix exponentiation to a 3-term recurrence (e.g. Tribonacci) - just grow the matrix to 3x3.
 *   - Compute phi for all numbers 1..n at once using a sieve-like approach (O(n log log n)) instead of
 *     calling eulerPhi(n) individually - needed when you have many queries.
 *   - CF problems tagged "combinatorics", "number theory", "matrices" on codeforces.com/problemset
 */
