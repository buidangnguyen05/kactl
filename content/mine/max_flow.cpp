// Find maximum flow in a network

struct Edge {
	int to, flow, cap;

	Edge() {}
	Edge(int _to, int _cap) {
		to = _to; cap = _cap;
		flow = 0;
	}
};

vector<Edge> edges;

const int N = 1e3 + 10;

struct MaxFlow {
	vector<int> adj[N];
	
	int source, sink, dist[N], nxt[N];

	void add_edge(int x, int y, int cap) {
		adj[x].push_back(edges.size());
		edges.emplace_back(Edge(y, cap));
		adj[y].push_back(edges.size());
		edges.emplace_back(Edge(x, 0));
	}

	bool bfs() {
		memset(dist, -1, sizeof(dist));
		dist[source] = 0;
		queue<int> q;
		q.push(source);

		while (q.size()) {
			int x = q.front(); q.pop();
			for (int i : adj[x]) if (edges[i].cap > edges[i].flow) {
				int v = edges[i].to;
				if (!~dist[v]) {
					dist[v] = dist[x] + 1;
					q.push(v);
				}
			}
		}

		return ~dist[sink];
	}

	int dfs(int x, int cur) {
		if (cur <= 0) return 0;
		if (x == sink) return cur;
		int ret = 0;
		for (; nxt[x] < (int)adj[x].size(); ++nxt[x]) {
			int i = adj[x][nxt[x]], v = edges[i].to, w = edges[i].cap - edges[i].flow;
			if (dist[v] != dist[x] + 1) continue;
			int u = dfs(v, min(w, cur));
			if (u) {
				edges[i].flow += u;
				edges[i ^ 1].flow -= u;
				ret += u; cur -= u;
			}
			if (!cur) break;
		}
		return ret;
	}

	int flow() {
		int ret = 0;
		while (bfs()) {
			memset(nxt, 0, sizeof(nxt));
			while (int x = dfs(source, 1e9)) ret += x;
		}
		return ret;
	}
} f;