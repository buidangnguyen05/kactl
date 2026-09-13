/**
 * Author: adamant
 * Description: Ordered set for integers in [0, N).
 * Time: O(n \cdot \log_2 n)
 * Status: stress-tested
 */

#pragma once

// If data type is not integer / not compressible into [0, N):
// typedef __gnu_pbds::tree<T, null_type,less<T>, rb_tree_tag,tree_order_statistics_node_update> ordered_set;
// needs to include <ext/pb_ds/assoc_container.hpp> AND <ext/pb_ds/tree_policy.hpp>

namespace cp_algo::structures {
    template<typename T, typename Container = std::vector<T>>
    struct fenwick {
        size_t n;
        Container data;

        fenwick(auto &&range) {
            assign(range);
        }
        void to_prefix_sums() {
            for(size_t i = 1; i < n; i++) {
                if(i + (i & -i) <= n) {
                    data[i + (i & -i)] += data[i];
                }
            }
        }
        void assign(auto &&range) {
            n = size(range) - 1;
            data = move(range);
            to_prefix_sums();
        }
        void add(size_t x, T const& v) {
            for(++x; x <= n; x += x & -x) {
                data[x] += v;
            }
        }
        // sum of [0, r)
        T prefix_sum(size_t r) const {
            assert(r <= n);
            T res = 0;
            for(; r; r -= r & -r) {
                res += data[r];
            }
            return res;
        }
        // sum of [l, r)
        T range_sum(size_t l, size_t r) const {
            return prefix_sum(r) - prefix_sum(l);
        }
        // First r s.t. prefix_sum(r) >= k
        // Assumes data[x] >= 0 for all x
        size_t prefix_lower_bound(T k) const {
            int x = 0;
            for(size_t i = std::bit_floor(n); i; i /= 2) {
                if(x + i <= n && data[x + i] < k) {
                    k -= data[x + i];
                    x += i;
                }
            }
            return x;
        }
    };
}

namespace cp_algo::structures {
    // fenwick-based set for [0, maxc)
    template<size_t maxc>
    struct fenwick_set: fenwick<int, std::array<int, maxc+1>> {
        using Base = fenwick<int, std::array<int, maxc+1>>;
        size_t sz = 0;
        std::bitset<maxc> present;
        fenwick_set(): Base(std::array<int, maxc+1>()) {}
        fenwick_set(auto &&range): fenwick_set() {
            for(auto x: range) {
                Base::data[x + 1] = 1;
                sz += !present[x];
                present[x] = 1;
            }
            Base::to_prefix_sums();
        }
        void insert(size_t x) {
            if(present[x]) return;
            present[x] = 1;
            sz++;
            Base::add(x, 1);
        }
        void erase(size_t x) {
            if(!present[x]) return;
            present[x] = 0;
            sz--;
            Base::add(x, -1);
        }
        size_t order_of_key(size_t x) const {
            return Base::prefix_sum(x);
        }
        size_t find_by_order(size_t order) const {
            return order < sz ? Base::prefix_lower_bound(order + 1) : -1;
        }
        size_t lower_bound(size_t x) const {
            if(present[x]) {return x;}
            auto order = order_of_key(x);
            return order < sz ? find_by_order(order) : -1;
        }
        size_t pre_upper_bound(size_t x) const {
            if(present[x]) {return x;}
            auto order = order_of_key(x);
            return order ? find_by_order(order - 1) : -1;
        }
    };
}