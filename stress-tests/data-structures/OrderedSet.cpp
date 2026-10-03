#include "../utilities/template.h"
#include "../../content/data-structures/OrderedSet.h"

const int N = 37;
OrderedSet<N> os;

int main() {
	srand(17);
	rep(it,0,300) {
		os = OrderedSet<N>();
		set<int> ref;
		rep(q,0,400) {
			int x = rand() % N;
			if (rand() & 1) os.insert(x), ref.insert(x);
			else os.erase(x), ref.erase(x);
			assert(os.size() == sz(ref));
			assert(os.count(x) == (bool)ref.count(x));
			vi v(all(ref));
			rep(k,0,N) {
				assert(os.order_of_key(k) ==
					(int)(lower_bound(all(v), k) - v.begin()));
				int e = k < sz(v) ? v[k] : -1;
				assert(os.find_by_order(k) == e);
				auto nx = lower_bound(all(v), k);
				assert(os.next(k) == (nx == v.end() ? -1 : *nx));
				auto pv = upper_bound(all(v), k);
				assert(os.prev(k) == (pv == v.begin() ? -1 : *--pv));
			}
		}
	}
	cout<<"Tests passed!"<<endl;
}
