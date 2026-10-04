#include "../utilities/template.h"

#include "../../content/data-structures/PalindromicTree.h"

bool pal(const string& w) { return w == string(w.rbegin(), w.rend()); }

int main() {
	srand(1);
	rep(it,0,3000) {
		int n = rand() % 30, k = rand() % 3 + 1;
		string s(n, 'a');
		for (char& c : s) c = char('a' + rand() % k);
		PalinTree t(s);
		map<string, int> occ; // every palindromic substring, with its count
		rep(i,0,n) rep(j,i,n) if (pal(s.substr(i, j - i + 1))) occ[s.substr(i, j - i + 1)]++;
		assert(t.n == sz(occ) + 2);
		map<string, int> id;
		rep(v,2,t.n) { // nodes are exactly the distinct palindromes
			string w = s.substr(t.pos[v], t.len[v]);
			assert(pal(w) && occ.count(w) && !id.count(w));
			id[w] = v;
			assert(t.freq[v] == occ[w]);
		}
		rep(v,2,t.n) { // fail = longest proper palindromic suffix, 1 if none
			string w = s.substr(t.pos[v], t.len[v]);
			int f = 1;
			for (int l = sz(w) - 1; l >= 1; l--)
				if (pal(w.substr(sz(w) - l))) { f = id[w.substr(sz(w) - l)]; break; }
			assert(t.fail[v] == f);
		}
	}
	string s(100000, 'a');
	for (char& c : s) c = char('a' + rand() % 2);
	PalinTree t(s);
	assert(t.n <= sz(s) + 2);
	cout << "Tests passed!" << endl;
}
