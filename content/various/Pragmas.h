/**
 * Author: frex-e
 * License: CC0
 * Source: https://github.com/frex-e/kactl
 * Description: Put above every include. \texttt{Ofast} implies
 *  \texttt{-ffast-math}, unsafe with tight eps. \texttt{avx2} faults on
 *  pre-2013 judges. \texttt{trapv} is for local debugging only.
 * Status: untested
 */
#pragma once

#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
// #pragma GCC optimize("trapv") // debug: abort on signed overflow
