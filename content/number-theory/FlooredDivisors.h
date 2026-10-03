/**
 * Author: unknown
 * Description: The maximal intervals $[L, R]$ on which $\lfloor n/i
 *  \rfloor$ is constant, collapsing $\sum_i f(\lfloor n/i \rfloor)$.
 * Time: O(\sqrt{n})
 * Status: stress-tested
 */
#pragma once

vector<pii> floors(int n) {
	vector<pii> ret;
	for (int L = 1, R; L <= n; L = R + 1) {
		R = n / (n / L); // every i in [L, R] has the same n / i
		ret.emplace_back(L, R);
	}
	return ret;
}
