//Given a string s, builds its suffix array + LCP array

const int N = 1e5 + 10;
int n, sa[N], pos[N], tmp[N], lcp[N], len;

namespace SuffixArray {
	bool cmp(int x, int y) {
		if (pos[x] != pos[y]) return pos[x] < pos[y];
		x += len; y += len;
		return (x <= n && y <= n) ? pos[x] < pos[y] : x > y;
	}

	void buildSA(string s) {
		n = s.size();
		s = '#' + s;

		for (int i = 1; i <= n; ++i) sa[i] = i, pos[i] = s[i];
		tmp[1] = 1;

		for (len = 1; ; len <<= 1) {
			sort (sa + 1, sa + n + 1, cmp);
			for (int i = 1; i < n; ++i) tmp[i + 1] = tmp[i] + cmp(sa[i], sa[i + 1]);
			for (int i = 1; i <= n; ++i) pos[sa[i]] = tmp[i];
			if (tmp[n] == n) break;
		}

		for (int i = 1, k = 0; i <= n; ++i) if (pos[i] != n) {
				for (int j = sa[pos[i] + 1]; s[i + k] == s[j + k]; ++k);
				lcp[pos[i]] = k;
				if (k) --k;
			}
	}
}
