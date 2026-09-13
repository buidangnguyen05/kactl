/**
 * Author: awk
 * Description: Applicable for problems required operations on XOR indexes (e.g: a[l \^{} x], a[(l + 1) \^{} x], ...) 
 * Query update is only available if Merge(id, id + 1) == Merge(id + 1, id) (Commutative)
 * Time: O(log(n)) per query
 */
#pragma once

#define pb push_back

const int maxN = 1e5 + 1;
int a[maxN];
struct XORSegtree {
    vector<int> st[4 * maxN];
    int lazy[4 * maxN];
    XORSegtree() {
        rep(i, 0, 4 * maxN) {
            st[i].clear();
            lazy[i] = 0;
        }
    }
    void build(int id, int l, int r){
        lazy[id] = 0;
        st[id].clear();
        if(l == r){
            st[id].pb(a[l]);
            return;
        }
        int mid = (l + r) / 2;
        build(id * 2, l, mid); build(id * 2 + 1, mid + 1, r);
        int len = r - l + 1;
        st[id].resize(len);
        rep(i, 0, len) {
            if(i >= len / 2) 
                st[id][i] = st[id * 2][i - len / 2] + st[id * 2 + 1][i - len / 2];
            else st[id][i] = st[id * 2 + 1][i] + st[id * 2][i];
        }
    }
    void update(int id, int l, int r, int pos, int val) {
        lazy[id] += val;
        if (l == r) return;
        int mid = (l + r) / 2;
        if (pos <= mid) update(id * 2, l, mid, pos, val);
        else update(id * 2 + 1, mid + 1, r, pos, val);
    }
    int get(int id, int l, int r, int u, int v, int x, int depth) {
        if(l > v || r < u) return 0;
        if(l >= u && r <= v){
            return lazy[id] + st[id][x & ((1 << depth) - 1)];
        }
        int mid = (l + r) / 2;
        if(x >> (depth - 1) & 1){
            return get(id * 2 + 1, mid + 1, r, u - l + (mid + 1), v - l + (mid + 1), x, depth - 1) 
            + get(id * 2, l, mid, l + u - (mid + 1), l + v - (mid + 1), x, depth - 1);
        }
        else {
            return get(id * 2, l, mid, u, v, x, depth - 1) 
            + get(id * 2 + 1, mid + 1, r, u, v, x, depth - 1);
        }
    }
};