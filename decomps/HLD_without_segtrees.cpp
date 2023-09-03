#include "template.h"

int a[100001], w[100001], up[100001], dth[100001], pred[100001];
int tin[100001], tout[100001], tmr;
vector<int> gr[100001];
bool mark[100001];
unordered_map<int, vector<int>> tree_pos[100001];

void dfs(int v, int pr) {
  tin[v] = tmr++;
  pred[v] = pr;
  dth[v] = dth[pr] + 1;
  int x, im = -1;
  for (int i = 0; i < sz(gr[v]); i++) {
    if (gr[v][i] == pr) continue;
    x = gr[v][i];
    dfs(x, v);
    w[v] += w[gr[v][i]];
    if (im == -1 || w[gr[v][i]] > w[gr[v][im]]) im = i;
  }
  if (im >= 0) mark[gr[v][im]] = true;
  w[v]++;
  tout[v] = tmr++;
}

bool is_anc(int x, int y) {
  return tin[x] <= tin[y] && tout[x] >= tout[y];
}

int main() {
  fast_io;
  //freopen("milkvisits.in", "r", stdin);
  //freopen("milkvisits.out", "w", stdout);
  int n, m, i, x, y, z;
  queue<pii> q;
  cin >> n >> m;
  for (i = 1; i <= n; i++) cin >> a[i];
  for (i = 1; i < n; i++) {
    cin >> x >> y;
    gr[x].pb(y), gr[y].pb(x);
  }
  dfs(1, 0);
  q.push({1, 0});
  while (!q.empty()) {
    x = q.front().first;
    y = q.front().second;
    q.pop();
    if (mark[x]) up[x] = up[y];
    else up[x] = x;
    tree_pos[up[x]][a[x]].pb(dth[x] - dth[up[x]] + 1);
    for (auto f : gr[x]) {
      if (f == y) continue;
      q.push({f, x});
    }
  }
  for (i = 1; i <= m; i++) {
    cin >> x >> y >> z;
    bool found = false;
    int p1;
    while (true) {
      if (up[x] != up[y]) {
        if (is_anc(up[x], y)) swap(x, y);
        if (tree_pos[up[x]].find(z) != tree_pos[up[x]].end()) {
          p1 = tree_pos[up[x]][z][0];
          if (p1 <= dth[x] - dth[up[x]] + 1) found = true;
        }
        if (found) break;
        x = pred[up[x]];
      } else {
        if (dth[x] < dth[y]) swap(x, y);
        int d1 = dth[y] - dth[up[x]] + 1;
        int d2 = dth[x] - dth[up[x]] + 1;
        if (tree_pos[up[x]].find(z) != tree_pos[up[x]].end()) {
          auto fi = lower_bound(all(tree_pos[up[x]][z]), d1);
          if (fi != tree_pos[up[x]][z].end() && *fi <= d2) found = true;
        }
        break;
      }
    }
    cout << found;
  }
  //cerr << '\n' << "Time execute: " << clock() / (double)CLOCKS_PER_SEC << " sec" << '\n';
  return 0;
}
