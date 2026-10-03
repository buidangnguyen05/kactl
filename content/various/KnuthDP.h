/**
 * Author: Simon Lindholm
 * License: CC0
 * Source: http://codeforces.com/blog/entry/8219
 * Description: When doing DP on intervals: $a[i][j] = \min_{i < k < j}(a[i][k] + a[k][j]) + f(i, j)$, where the (minimal) optimal $k$ increases with both $i$ and $j$,
 *  one can solve intervals in increasing order of length, and search $k = p[i][j]$ for $a[i][j]$ only between $p[i][j-1]$ and $p[i+1][j]$.
 *  This is known as Knuth DP. Sufficient criteria for this are if $f(b,c) \le f(a,d)$ and $f(a,c) + f(b,d) \le f(a,d) + f(b,c)$ for all $a \le b \le c \le d$.
 *  Consider also: LineContainer (ch. Data structures), monotone queues, ternary search.
 * Time: O(N^2)
 * Status: stress-tested against the cubic DP
 */
#pragma once

// Half-open: dp[i][j] covers items [i, j), and dp[i][i+1] = 0.
// C(i, j) is the cost of the final merge; for merging stones with
// prefix sums pre, C = [&](int i, int j) { return pre[j]-pre[i]; }.
template<class F> ll knuth(int n, F C) {
	vector<vector<ll>> dp(n + 1, vector<ll>(n + 1));
	vector<vi> opt(n + 1, vi(n + 1));
	rep(i,0,n) opt[i][i+1] = i;
	for (int len = 2; len <= n; len++) rep(i,0,n-len+1) {
		int j = i + len;
		dp[i][j] = LLONG_MAX;
		rep(k, max(i+1, opt[i][j-1]), min(j-1, opt[i+1][j]) + 1) {
			ll v = dp[i][k] + dp[k][j] + C(i, j);
			if (v < dp[i][j]) dp[i][j] = v, opt[i][j] = k;
		}
	}
	return dp[0][n];
}
