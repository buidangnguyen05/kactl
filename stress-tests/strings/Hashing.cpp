#include "../utilities/template.h"

#include "../../content/strings/Hashing.h"

// Helpers formerly printed in Hashing.h, kept to test H.
struct HashInterval {
	vector<H> ha, pw;
	HashInterval(string& str) : ha(sz(str)+1), pw(ha) {
		pw[0] = 1;
		rep(i,0,sz(str))
			ha[i+1] = ha[i] * C + str[i],
			pw[i+1] = pw[i] * C;
	}
	H hashInterval(int a, int b) { // hash [a, b)
		return ha[b] - ha[a] * pw[b - a];
	}
};

vector<H> getHashes(string& str, int length) {
	if (sz(str) < length) return {};
	H h = 0, pw = 1;
	rep(i,0,length)
		h = h * C + str[i], pw = pw * C;
	vector<H> ret = {h};
	rep(i,length,sz(str)) {
		ret.push_back(h = h * C + str[i] - pw * str[i-length]);
	}
	return ret;
}

H hashString(string& s){H h{}; for(char c:s) h=h*C+c;return h;}

#include <sys/time.h>
int main() {
	assert((H(1)*2+1-3).get() == 0);

	rep(it,0,10000) {
		int n = rand() % 10;
		int alpha = rand() % 10 + 1;
		string s;
		rep(i,0,n) s += (char)('a' + rand() % alpha);
		HashInterval hi(s);
		set<string> strs;
		set<ull> hashes;

		// HashInterval
		rep(i,0,n+1) rep(j,i,n+1) {
			string sub = s.substr(i, j - i);
			ull hash = hashString(sub).get();
			assert(hi.hashInterval(i, j).get() == hash);
			hashes.insert(hash);
			strs.insert(sub);
		}

		// getHashes
		rep(le,1,n+1) {
			auto ve = getHashes(s, le);
			assert(sz(ve) == n-le+1);
			rep(i,0,n-le+1) {
				assert(ve[i].get() == hi.hashInterval(i, i + le).get());
			}
		}

		// No collisions
		assert(sz(strs) == sz(hashes));
	}
	cout<<"Tests passed!"<<endl;
}
