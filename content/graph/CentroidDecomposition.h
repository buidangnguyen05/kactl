/**
 * Author: buidangnguyen05
 * Description: Divide-and-conquer on a tree.
 * Time: O(n \cdot \log_2 n) (with high constant factor)
 */

#pragma once
const int N = 2e5 + 10;
int sz[N]; bool used[N];
vector<int> adj[N];
int calc(int x, int par) {
    sz[x] = 1;
    for (int &i : adj[x]) if (!used[i] && i != par) sz[x] += calc(i,x);
    return sz[x];
}
int getCentroid(int x, int par, int cnt) {
    for (int &i : adj[x]) if (!used[i] && i != par && sz[i] * 2 > cnt) return getCentroid(i, x, cnt);
    return x;
}
void centroid(int x, int par = 0) {
    int cnt = calc(x, x), node = getCentroid(x, x, cnt);
    // do work here
    used[node] = 1; 
    for (int &i : adj[node])  if (!used[i]) centroid(i, node);
}