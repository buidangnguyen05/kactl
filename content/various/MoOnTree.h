/**
 * Author: Epiphyllum
 * Description: Path query to range query. Push $v$ on entry and exit,
 *  giving $st[v] < ed[v]$ in a tour of length $2n$. For $u \dots v$ with
 *  $st[u] \le st[v]$: $[st[u], st[v]]$ if $u$ is an ancestor, else
 *  $[ed[u], st[v]]$ plus $lca(u,v)$. Off-path vertices occur twice, so
 *  make add() toggle and count only odd occurrences.
 * Time: O((n + q)\sqrt n)
 * Status: stress-tested
 */
#pragma once
