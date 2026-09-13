/**
 * Author: Epiphyllum
 * Description: Actual n / 64 bitset.
 * Time: O(\frac{n}{64})
 * Status: stress-tested
 */

#pragma once

struct bit {
    const static int L = 782;
    ull t[L];
    void set(int x, int y = 1) {
        if (y) t[x >> 6] |= 1ull << (x & 63);
        else t[x >> 6] &= t[x >> 6] ^(1ull << (x & 63));
    }
    void reset() {
        memset(t, 0, sizeof(t));
    }
    void flip(int x) {
        t[x >> 6] ^= 1ull << (x & 63);
    }
    int test(int x) {
        return t[x >> 6] >> (x & 63) & 1;
    }
    ull& operator [](int i) {
        return t[i];
    }
    int count() {
        int res=0;
        rep(i, 0, L) res += __builtin_popcountll(t[i]);
        return res;
    }
    friend bit operator ^(const bit &a, const bit &b) {
        bit tmp;
        rep(i, 0, L) tmp.t[i] = a.t[i] ^b.t[i];
        return tmp;
    }
    friend bit operator & (const bit &a, const bit &b) {
        bit tmp;
        rep(i, 0, L) tmp.t[i] = a.t[i] & b.t[i];
        return tmp;
    }
    friend bit operator | (const bit &a, const bit &b) {
        bit tmp;
        rep(i, 0, L) tmp.t[i] = a.t[i] | b.t[i];
        return tmp;
    }
};
