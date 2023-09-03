#include "template.h"

unordered_set<int> gr[100001];
unordered_map<int, int> dist[100001];
int inf = 1e9, sz[100001], dth[100001], rasst[100001];
vector<int> centroidpred[100001];
bool used[100001];

void go(int x, int y) {
  for (auto f: gr[x]) {
    if (f == y) continue;
    dth[f] = dth[x] + 1;
    go(f, x);
  }
}

void cnt(int x, int y, int st) {
  //cout << x << ' ' << y << ' ' << st << '\n';
  if (x != st) dist[st][x] = dist[st][y] + 1;
  for (auto f: gr[x]) {
    if (used[f] || f == y) continue;
    cnt(f, x, st);
  }
}

void dfs(int x, int y) {
  sz[x] = 0;
  for (auto f: gr[x]) {
    if (used[f] || f == y) continue;
    dfs(f, x);
    sz[x] += sz[f];
  }
  sz[x]++;
}

int find_centroid(int x, int y, int st) {
  int mx = 0, ind;
  for (auto f: gr[x])
    if (!used[f] && f != y && sz[f] > sz[st] / 2 && sz[f] > mx) mx = sz[f], ind = f;
  return (mx > 0 ? find_centroid(ind, x, st) : x);
}

void color(int x) {
  for (auto f: centroidpred[x])
    rasst[f] = min(rasst[f], dist[f][x]);
}

int main() {
  fast_io;
  int n, m, i, x, y, z, ans;
  cin >> n >> m;
  for (i = 1; i < n; i++) {
    cin >> x >> y;
    gr[x].insert(y);
    gr[y].insert(x);
  }
  for (i = 1; i <= n; i++) rasst[i] = inf;
  go(1, 0);
  queue<pii> q;
  q.push({1, 0});
  while (!q.empty()) {
    x = q.front().first;
    y = q.front().second;
    dfs(x, y);
    q.pop();
    z = find_centroid(x, y, x);
    used[z] = 1;
    centroidpred[z] = centroidpred[y];
    centroidpred[z].pb(z);
    dist[z][z] = 0;
    cnt(z, 0, z);
    for (auto f: gr[z]) {
      if (used[f]) continue;
      gr[f].erase(z), q.push({f, z});
    }
    gr[z].clear();
  }
  color(1);
  for (i = 1; i <= m; i++) {
    cin >> x >> y;
    if (x == 1) color(y);
    else {
      ans = inf;
      for (auto f: centroidpred[y])
        ans = min(ans, dist[f][y] + rasst[f]);
      cout << ans << '\n';
    }
  }
  return 0;
}
