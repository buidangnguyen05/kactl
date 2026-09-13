/**
 * Author: awk
 * Description: Given an array of n elements with q queries [l, r, k]. For each query print the k-th smallest value and the sum of k smallest values in the subsequence [l, r]. Calculate the position of k in the subsequence [l, r].
 * Time: O(n * log(a_{max}))
 */

struct WaveletTree{
    struct TNode{
        int l = 0, r = 0, lp = 0, rp = 0;
        TNode(int _l, int _r) : l(_l), r(_r) {}
    };
    vector <vector <int>> val;
    vector <vector <ll>> sum;
    vector <int> b;
    vector <TNode> wt;
    int wt_sz = 1;
    WaveletTree(int _n = 0) : val(_n * 4), sum(_n * 4), wt(_n * 4) {}
    template <class T> void build(int id, T i, T j, int l, int r){
        wt[id].l = ll; wt[id].r = r;
        if (l == r) return;
        val[id].pb(0); sum[id].pb(0);
        int mid = (l + r - (l + r < 0)) / 2, maxx = l, minn = r;
        for (auto it = i; it != j; ++it){
            val[id].pb(val[id].back() + (*it <= mid));
            sum[id].pb(sum[id].back() + *it * (*it <= mid));
            if (*it <= mid) maxx = max(maxx, *it);
            else minn = min(minn, *it);
        }
        auto p = stable_partition(i, j, [=](const auto &w) {return w <= mid;});
        wt[id].lp = ++wt_sz; wt[id].rp = ++wt_sz;
        build(wt[id].lp, i, p, l, maxx); build(wt[id].rp, p, j, minn, r);
    }
    ll pos(int k, int i, int j) {
        --i;
        ll cnt = 0; int id = 1, l = wt[id].l, r = wt[id].r;
        while (l < r){
            int mid = (l + r - (l + r < 0)) / 2;
            if (k <= mid) i = val[id][i], j = val[id][j], id = wt[id].lp;
            else{
                cnt += val[id][j] - val[id][i];
                i -= val[id][i], j -= val[id][j], id = wt[id].rp;
            }
            l = wt[id].l, r = wt[id].r;
        }
        return cnt + (k >= l) * (j - i);
    }
    ll get(int k, int i, int j){
        if (!k) return 0;
        --i;
        ll res = 0; int id = 1, l = wt[id].l, r = wt[id].r;
        while (l < r) {
            if (k <= val[id][j] - val[id][i]) i = val[id][i], j = val[id][j], id = wt[id].lp;
            else{
                res += sum[id][j] - sum[id][i];
                k -= val[id][j] - val[id][i];
                i -= val[id][i], j -= val[id][j], id = wt[id].rp;
            }
            l = wt[id].l, r = wt[id].r;
        }
        return res + k * l;
    }
};
