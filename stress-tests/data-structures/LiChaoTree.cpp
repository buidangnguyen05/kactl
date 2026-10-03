#include "../utilities/template.h"
#include "../../content/data-structures/LiChaoTree.h"

int main() {
	srand(7);
	rep(it,0,400) {
		int n = rand() % 30 + 1;
		LiChao lc(n);
		vector<array<ll,4>> segs; // a, b, u, v
		rep(q,0,20) {
			ll a = rand() % 21 - 10, b = rand() % 41 - 20;
			int u = rand() % n, v = rand() % n;
			if (u > v) swap(u, v);
			if (rand() & 1) u = 0, v = n - 1;
			lc.addSeg({a, b}, u, v);
			segs.push_back({a, b, u, v});
			rep(x,0,n) {
				ll want = Line::INF;
				for (auto& s : segs)
					if (s[2] <= x && x <= s[3])
						want = min(want, s[0] * x + s[1]);
				assert(lc.query(x) == want);
			}
		}
	}
	cout<<"Tests passed!"<<endl;
}
