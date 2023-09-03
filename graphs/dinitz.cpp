#include "template.h"

unordered_map<int, ll> ump[200001];
struct edge { int nxt; ll cap; int id_rev; };
int d[200001], p[200001], q[200001], n, m, id_free = 1;
edge edges_info[400002];
vector<int> edges_id[200001];

bool bfs() {
  memset(d, -1, sizeof(d));
  int x, v, cpos = 0, npos = 0;
  q[npos++] = 1, d[1] = 0;
  while (cpos < npos && d[n] == -1) {
    x = q[cpos++];
    for (const auto& f : edges_id[x]) {
      v = edges_info[f].nxt;
      if (edges_info[f].cap && d[v] == -1) {
        d[v] = d[x] + 1;
        q[npos++] = v;
      }
    }
  }
  return d[n] != -1;
}

ll dfs(int x, ll min_cap) {
  if (x == n || min_cap == 0) {
    return min_cap;
  }
  int id, v;
  ll diff, cap;
  while (p[x] < edges_id[x].size()) {
    id = edges_id[x][p[x]];
    cap = edges_info[id].cap;
    if (!cap) {
      p[x]++;
      continue;
    }
    v = edges_info[id].nxt;
    if (d[v] == d[x] + 1) {
      diff = dfs(v, min(min_cap, cap));
      if (diff) {
        edges_info[id].cap -= diff;
        edges_info[edges_info[id].id_rev].cap += diff;
        return diff;
      }
    }
    p[x]++;
  }
  return 0;
}

int main() {
  fast_io;
  int i, u, v, w;
  cin >> n >> m;
  for (i = 1; i <= m; i++) {
    cin >> u >> v >> w;
    ump[u][v] += w;
  }
  for (i = 1; i <= n; i++) {
    for (const auto& f : ump[i]) {
      edges_id[i].push_back(id_free);
      edges_info[id_free] = {f.first, f.second, ++id_free};
      edges_id[f.first].push_back(id_free);
      if (ump[f.first].find(i) != ump[f.first].end()) {
        edges_info[id_free] = {i, ump[f.first][i], id_free - 1};
        ump[f.first].erase(i);
      } else edges_info[id_free] = {i, 0, id_free - 1};
      id_free++;
    }
  }
  long long max_flow = 0, cur_flow, inf = 1e18;
  while (bfs()) {
    memset(p, 0, sizeof(p));
    cur_flow = dfs(1, inf);
    while (cur_flow) {
      max_flow += cur_flow;
      cur_flow = dfs(1, inf);
    }
  }
  cout << max_flow;
  return 0;
}
