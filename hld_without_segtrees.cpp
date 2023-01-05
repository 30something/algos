#include <bits/stdc++.h>
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>

#define ld long double
#define ll long long
#define ull unsigned long long
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pld pair<ld, ld>
#define pb push_back
#define ppb pop_back
#define eb emplace_back
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) ((int)(x).size())
#define fast_io cin.tie(0), ios_base::sync_with_stdio(0)

//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,abm,avx,avx2,fma,bmi,bmi2,lzcnt,popcnt")
//#pragma GCC optimize("Ofast", "unroll-loops")
//#pragma GCC optimize("O3")

using namespace std;
//using namespace __gnu_pbds;
//typedef tree<ll, null_type, less_equal<>, rb_tree_tag,
//    tree_order_statistics_node_update> ordered_set;

ld eps = 1. / 1e9;
ll inf_ll = 1e18 + 100, mod1 = 1e9 + 7, mod2 = 998244353;
ll mersen_mod = 2305843009213693951;
mt19937 gen32(chrono::steady_clock::now().time_since_epoch().count());

ll sqr(ll a) { return a * a; }
ll gcd(ll a, ll b) { return !a ? b : gcd(b % a, a); }
ll add(ll a, ll b, ll mod) {
  a += b;
  if (a >= mod) a -= mod;
  return a;
}
ll sub(ll a, ll b, ll mod) {
  a -= b;
  if (a < 0) a += mod;
  return a;
}
ll binpow(ll a, ll b, ll mod) {
  return b ? (b % 2 ? (a * (sqr(binpow(a, b / 2, mod)) % mod)) % mod :
              sqr(binpow(a, b / 2, mod)) % mod) : 1;
}
ll binmult(ll a, ll b, ll mod) {
  return b ? (b % 2 ? (2 * binmult(a, b / 2, mod) + a) % mod :
              (2 * binmult(a, b / 2, mod)) % mod) : 0;
}

/// Code here
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
