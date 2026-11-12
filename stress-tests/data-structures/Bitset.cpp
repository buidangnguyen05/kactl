#include "../utilities/template.h"

#include "../../content/data-structures/Bitset.h"

const int N = bit::L * 64;
bit a, b;
bitset<N> A, B;

void same() {
	rep(i,0,N) assert(a.test(i) == A[i] && b.test(i) == B[i]);
	assert(a.count() == (int)A.count());
	assert(a.find_first() == (int)A._Find_first()); // N if none
	rep(w,0,bit::L) rep(j,0,64) assert((a[w] >> j & 1) == A[w * 64 + j]);
}

int main() {
	srand(2);
	rep(round,0,60) {
		int density = rand() % 4; // 0: sparse, so find_first sees empty words
		rep(it,0,4000) {
			int x = rand() % N, op = rand() % 6;
			if (density == 0 && op < 2 && rand() % 50) op = 1;
			if (op == 0) a.set(x), A.set(x);
			else if (op == 1) a.set(x, 0), A.reset(x);
			else if (op == 2) a.flip(x), A.flip(x);
			else if (op == 3) b.set(x), B.set(x);
			else if (op == 4) b.flip(x), B.flip(x);
			else assert(a.test(x) == A[x]);
		}
		same();
		int op = rand() % 4;
		if (op == 0) a = a ^ b, A ^= B;
		else if (op == 1) a = a & b, A &= B;
		else if (op == 2) a = a | b, A |= B;
		else a.reset(), A.reset();
		same();
		if (round % 10 == 9) { // word access writes through
			int w = rand() % bit::L;
			a[w] = 0, A &= ~(bitset<N>(~0ull) << (w * 64));
			same();
		}
	}
	a.reset(), A.reset();
	assert(a.find_first() == N && a.count() == 0);
	cout << "Tests passed!" << endl;
}
