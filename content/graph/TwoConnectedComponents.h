/**
 * Author: buidangnguyen05
 * Description: Lists out all edge biconnected components in a graph (components that if any edge is deleted is still connected).
 * Time: O(E + V)
 */

#pragma once

vector<vector<pair<int, int>>> adj; 
vector<int> num, low, color, id_twoconected; 
vector<vector<int>> two_conected; 
int cnt_twoconected, Time; 
 
stack<int> st; 
void DFS(int u, int edge_dad) { 
    num[u] = low[u] = ++Time; 
    color[u] = 1; 
    st.push(u); 
    for(auto e : adj[u]) { 
        int v = e.first, edge_id = e.second; 
        if (edge_id != edge_dad) { 
            if (color[v] == 0) { 
                DFS(v, edge_id); 
                low[u] = min(low[u], low[v]); 
            } else low[u] = min(low[u], num[v]); 
        } 
    } 
    if (low[u] == num[u]) { 
        ++cnt_twoconected; 
        int v; 
        do { 
            v = st.top(); 
            st.pop(); 
            id_twoconected[v] = cnt_twoconected; 
            two_conected[cnt_twoconected].push_back(v); 
        } while (v != u); 
    } 
} 

