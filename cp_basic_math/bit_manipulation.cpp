/*
 * BIT MANIPULATION
 * ------------------
 * Shows up constantly on CF: bitmask DP, subset enumeration, XOR tricks,
 * fast set operations. Most people "know" bit tricks exist but freeze when
 * asked to actually write them - this file is the reference to fix that.
 *
 * Covered:
 *   1. Basic bit operations (check/set/clear/toggle a bit)
 *   2. Built-in GCC bit functions (popcount, ctz, clz) - use these, don't hand-roll them
 *   3. Iterate over all subsets of a bitmask (the "submask enumeration" trick)
 *   4. Iterate over all subsets of size k / all bitmasks with k bits set
 *   5. XOR tricks: find the single non-duplicate, swap without temp variable
 *   6. Bitmask DP: Traveling Salesman Problem (TSP) - the classic bitmask DP application
 *
 * Compile: g++ -std=c++17 -O2 -o bit bit_manipulation.cpp && ./bit
 */

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    // ---------------- 1) Basic bit operations ----------------
    int x = 0b10110; // 22 in decimal
    cout << "=== Basic bit ops on x = 0b10110 (22) ===\n";

    // check if bit i is set: (x >> i) & 1
    cout << "bit 1 set? " << ((x >> 1) & 1) << " (expected 1)\n";
    cout << "bit 0 set? " << ((x >> 0) & 1) << " (expected 0)\n";

    // set bit i: x | (1 << i)
    int setBit = x | (1 << 0);
    cout << "set bit 0: " << bitset<8>(setBit) << " (expected 00010111)\n";

    // clear bit i: x & ~(1 << i)
    int clearBit = x & ~(1 << 1);
    cout << "clear bit 1: " << bitset<8>(clearBit) << " (expected 00010100)\n";

    // toggle bit i: x ^ (1 << i)
    int toggleBit = x ^ (1 << 2);
    cout << "toggle bit 2: " << bitset<8>(toggleBit) << " (expected 00010010)\n";

    // check if x is a power of 2: x & (x-1) == 0 (clears the lowest set bit;
    // a power of 2 has exactly one set bit, so clearing it gives 0)
    for (int v : {16, 18}) {
        cout << v << " is power of 2? " << ((v & (v - 1)) == 0) << "\n";
    }
    cout << "\n";

    // ---------------- 2) Built-in GCC bit functions - USE THESE instead of manual loops ----------------
    unsigned int n = 22; // 0b10110
    cout << "=== Built-ins on n = 22 (0b10110) ===\n";
    cout << "__builtin_popcount(n): " << __builtin_popcount(n) << " (count of set bits, expected 3)\n";
    cout << "__builtin_ctz(n): " << __builtin_ctz(n) << " (count trailing zeros, expected 1)\n";
    cout << "__builtin_clz(n): " << __builtin_clz(n) << " (count leading zeros in 32-bit int)\n";
    // for long long, use the _ll suffixed versions:
    ll bign = 1LL << 40;
    cout << "__builtin_popcountll(1LL<<40): " << __builtin_popcountll(bign) << " (expected 1)\n\n";

    // ---------------- 3) Iterate over ALL SUBSETS of a bitmask ----------------
    // Classic trick: for (int sub = mask; sub > 0; sub = (sub-1) & mask)
    // This visits every subset of `mask` (including mask itself), in decreasing order,
    // and takes O(3^n) total across all masks of n bits (each element is in 3 states:
    // "not in mask", "in mask but not in subset", "in subset").
    int mask = 0b1010; // subsets of {bit1, bit3}
    cout << "=== Subsets of mask=0b1010 ===\n";
    for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
        cout << bitset<4>(sub) << " ";
    }
    cout << "(0 aka empty set is excluded by the loop condition; expected 1010 1000 0010)\n\n";

    // ---------------- 4) Iterate all bitmasks of n bits with exactly k bits set ----------------
    // Uses Gosper's hack: given a bitmask with k bits set, compute the NEXT larger
    // bitmask (in numeric order) that also has exactly k bits set.
    auto nextSameBits = [](int v) {
        int c = v & -v;               // isolate lowest set bit
        int r = v + c;                // "carry" it into the next position
        return (((r ^ v) >> 2) / c) | r;  // fix up the trailing bits
    };
    cout << "=== All 4-bit masks with exactly 2 bits set ===\n";
    int v = 0b0011; // smallest 4-bit number with 2 bits set
    int limit = 1 << 4;
    while (v < limit) {
        cout << bitset<4>(v) << " ";
        v = nextSameBits(v);
    }
    cout << "(expected 0011 0101 0110 1001 1010 1100)\n\n";

    // ---------------- 5) XOR tricks ----------------
    // (a) Find the single number that doesn't repeat when everything else appears twice.
    //     XOR of a number with itself is 0, and XOR is commutative/associative, so
    //     XOR-ing everything cancels out all the pairs, leaving just the unique one.
    vector<int> nums = {4, 1, 2, 1, 2};
    int unique = 0;
    for (int num : nums) unique ^= num;
    cout << "=== XOR tricks ===\n";
    cout << "Single non-duplicate in {4,1,2,1,2}: " << unique << " (expected 4)\n";

    // (b) Swap two variables without a temp variable (rarely needed but a classic trick)
    int a = 5, b = 9;
    a ^= b; b ^= a; a ^= b;
    cout << "After XOR swap: a=" << a << " b=" << b << " (expected a=9 b=5)\n\n";

    return 0;
}

/*
 * BITMASK DP EXAMPLE: Traveling Salesman Problem (TSP), separate for clarity
 * (see the function below main() would normally not compile - kept as a
 * standalone comment-explained snippet you can lift into your own file):
 *
 *   dp[mask][i] = minimum cost to have visited exactly the set of cities in `mask`,
 *                 currently standing at city i.
 *   Transition: dp[mask | (1<<j)][j] = min(dp[mask][i] + cost[i][j]) for all j not in mask.
 *   Answer: min over i of dp[(1<<n)-1][i] + cost[i][0] (return to start).
 *   Complexity: O(2^n * n^2) - feasible for n up to ~18-20.
 *
 * TRY YOURSELF:
 *   - Implement full bitmask DP TSP given a cost matrix (great practice combining DP + bitmasks).
 *   - "Partition to K Equal Sum Subsets" (LeetCode 698) - bitmask DP over which numbers are used.
 *   - CF problems tagged "bitmasks" on codeforces.com/problemset
 */
