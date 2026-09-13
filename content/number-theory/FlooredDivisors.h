/**
 * Author: unknown
 * Description: Returns a vector of maximum intervals [L, R] such that $\lfloor \frac{n}{L} \rfloor = \lfloor \frac{n}{R} \rfloor$.
 * Time: O(2\sqrt{n})
 * Status: stress-tested
 */

vector<pii> floors(int x) {
    vector<pii> ret;
    for (int L = 1, R; L <= n; L = R + 1) {
        R = n / (n / L); // every i in [L, R] has the same n / i
        ret.emplace_back(L, R);
    }
    return ret;
}