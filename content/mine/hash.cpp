struct Hash {
    vector<int> h, power; bool rev;

    void build(string s, int base, bool _rev) {
        h.resize(s.size() + 10), power.resize(s.size() + 10); rev = _rev;
        power[0] = 1;
        for (int i = 1; i <= s.size(); ++i) power[i] = 1LL * power[i - 1] * base % mod;
        if (!rev) for (int i = 1; i <= s.size(); ++i) h[i] = (1LL * h[i - 1] * base + s[i - 1] - 'a' + 1) % mod;
        else for (int i = s.size(); i; --i) h[i] = (1LL * h[i + 1] * base + s[i - 1] - 'a' + 1) % mod;
    }
    
    int get(int L, int R) {
        if (!rev) return (0LL + h[R] - 1LL * h[L - 1] * power[R - L + 1] + 1LL * mod * mod) % mod;
        else return (0LL + h[L] - 1LL * h[R + 1] * power[R - L + 1] + 1LL * mod * mod) % mod;
    }
} H;