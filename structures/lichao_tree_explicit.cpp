// https://csacademy.com/contest/archive/task/squared-ends

#include <bits/stdc++.h>

#define ld long double
#define ll long long
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb push_back
#define ppb pop_back
#define eb emplace_back
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) ((int)(x).size())
#define fast_io cin.tie(0), ios_base::sync_with_stdio(0)

using namespace std;

ld eps = 1e-9;
ll inf_ll = 1e18 + 100, mod1 = 1e9 + 7, mod2 = 998244353;
mt19937 gen32(chrono::steady_clock::now().time_since_epoch().count());

ll sqr(ll a) { return a * a; }
ll gcd(ll a, ll b) { return !a ? b : gcd(b % a, a); }
ll binpow(ll a, ll b, ll mod) {
  return b ? (b % 2 ? (a * (sqr(binpow(a, b / 2, mod)) % mod)) % mod :
              sqr(binpow(a, b / 2, mod)) % mod) : 1;
}

ll a[10001], dp[10001][101];
pll line[4000100], tmp;

void build(ll l, ll r, ll v) {
  line[v] = {0, inf_ll};
  if (l == r) return;
  ll m = (l + r) / 2;
  build(l, m, 2 * v);
  build(m + 1, r, 2 * v + 1);
}

void add(ll l, ll r, ll v, ll k, ll b) {
  if (l == r) {
    ll old = line[v].first * l + line[v].second;
    ll nw = k * l + b;
    if (nw < old) line[v] = {k, b};
    return;
  }
  ll m = (l + r) / 2;
  ll curm = line[v].first * m + line[v].second;
  ll newm = k * m + b;
  ll curl = line[v].first * l + line[v].second;
  ll newl = k * l + b;
  if (newm < curm) tmp = line[v], line[v] = {k, b}, k = tmp.first, b = tmp.second;
  bool left = (newl <= curl), mid = (newm <= curm);
  if (left != mid) add(l, m, 2 * v, k, b);
  else add(m + 1, r, 2 * v + 1, k, b);
}

ll get(ll l, ll r, ll v, ll x) {
  if (l == r) return line[v].first * l + line[v].second;
  ll m = (l + r) / 2;
  ll cur = line[v].first * x + line[v].second;
  if (x <= m) return min(cur, get(l, m, 2 * v, x));
  else return min(cur, get(m + 1, r, 2 * v + 1, x));
}

int main() {
  fast_io;

  ll n, k, i, p;
  ll LB = 0, RB = 1000001, y;
  cin >> n >> k;
  for (i = 1; i <= n; i++) cin >> a[i], dp[i][1] = (a[i] - a[1]) * (a[i] - a[1]);
  for (p = 2; p <= k; p++) {
    build(LB, RB, 1);
    add(LB, RB, 1, -2 * a[p], a[p] * a[p]);
    for (i = p + 1; i <= n; i++) {
      y = get(LB, RB, 1, a[i]);
      dp[i][p] = y == inf_ll ? dp[i - 1][p - 1] : min(dp[i - 1][p - 1], y + a[i] * a[i]);
      add(LB, RB, 1, -2 * a[i], dp[i - 1][p - 1] + a[i] * a[i]);
      // cout << i << ' ' << p << ' ' << dp[i][p] << ' ' << y << '\n';
    }
  }
  cout << dp[n][k];

  return 0;
}
