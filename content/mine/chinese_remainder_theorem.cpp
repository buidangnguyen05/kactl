void normalize(ll &x, ll mod) {
	x %= mod;
	if (x < 0) x += mod;
}

ll px, py, pd;
void ExEuclid(ll a, ll b) {
	if (b == 0) {
		px = 1; py = 0; pd = a;
		return;
	}
	ExEuclid(b, a % b);
	ll x1 = px, y1 = py;
	px = y1, py = x1 - a / b * y1;
}

struct ChineseRemainderTheorem {
	ll ans, lcm;

	ll x, y, d;
	void ExtendedEuclidean(ll a, ll b) {
		if (b == 0) {
			x = 1; y = 0; d = a;
			return;
		}
		ExtendedEuclidean(b, a % b);
		ll x1 = x, y1 = y;
		x = y1, y = x1 - a / b * y1;
	}

	void add(ll a, ll n) {
		ExtendedEuclidean(lcm, n);
		assert((a - ans) % d == 0);
		ans = ans + x * (a - ans) / d % (n / d) * lcm; 
		lcm = lcm * n / d; // beware of overflow, use ModMulLL if necessary
		normalize(ans, lcm);
	}
};

struct FactorData {
	int cnt_base = 0, non_zero = 1, mod, base, lim;

	int pw(const int &x, int y) {
		if (!y) return 1;
		int mid = pw(x, y / 2);
		if (y & 1) return 1LL * mid * mid % mod * x % mod;
		return 1LL * mid * mid % mod;
	}

	void add(int x) {
		while (x % base == 0) {
			++cnt_base;
			x /= base;
		}
		non_zero = 1LL * non_zero * x % mod;
	}

	void del(int x) {
		while (x % base == 0) {
			--cnt_base;
			x /= base;
		}
		if (x == 1) return;
		ExEuclid(x, mod);
		non_zero = 1LL * non_zero * px % mod;
		if (non_zero < 0) non_zero += mod;
	}

	int get() {
		if (cnt_base >= lim) return 0;
		return 1LL * non_zero * pw(base, cnt_base) % mod;
	}
} factors[7];