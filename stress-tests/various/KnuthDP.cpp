#include "../utilities/template.h"
#include "../../content/various/KnuthDP.h"

int main() {
	srand(31);
	rep(it,0,2000) {
		int n = rand() % 9 + 1;
		vi a(n); rep(i,0,n) a[i] = rand() % 20 + 1;
		vector<ll> pre(n + 1, 0);
		rep(i,0,n) pre[i+1] = pre[i] + a[i];
		auto C = [&](int i, int j) { return pre[j] - pre[i]; };
		// plain cubic interval DP
		vector<vector<ll>> dp(n+1, vector<ll>(n+1, 0));
		for (int len = 2; len <= n; len++) rep(i,0,n-len+1) {
			int j = i + len;
			dp[i][j] = LLONG_MAX;
			rep(k,i+1,j)
				dp[i][j] = min(dp[i][j], dp[i][k]+dp[k][j]+C(i,j));
		}
		assert(knuth(n, C) == dp[0][n]);
	}
	cout<<"Tests passed!"<<endl;
}
