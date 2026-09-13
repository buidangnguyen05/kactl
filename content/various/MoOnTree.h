/**
 * Author: Epiphyllum
 * Description: Do DFS traversal to get in and out times (ed[x] = ++it, not ed[x] = it). If x = lca(x, y), query = [st[x], st[y]], else query = [ed[x], st[y]] + st[lca(x, y)]. Nodes that appear an even number of times should not be counted.
 * Time: O(n \cdot \sqrt(n)).
 * Status: stress-tested
 */
#pragma once