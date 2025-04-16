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

struct node {
  pll line;
  node* left = nullptr;
  node* right = nullptr;
};

ll a[10001], dp[10001][101];
pll tmp;

void refresh(node* curnode, ll l, ll r) {
  curnode->line = {0, inf_ll};
  if (l == r) return;
  ll m = (l + r) / 2;
  if (curnode->left != nullptr) refresh(curnode->left, l, m);
  if (curnode->right != nullptr) refresh(curnode->right, m + 1, r);
}

void add(node* curnode, ll l, ll r, ll k, ll b) {
  if (l == r) {
    ll old = curnode->line.first * l + curnode->line.second;
    ll nw = k * l + b;
    if (nw < old) curnode->line = {k, b};
    return;
  }
  ll m = (l + r) / 2;
  ll curm = curnode->line.first * m + curnode->line.second;
  ll newm = k * m + b;
  ll curl = curnode->line.first * l + curnode->line.second;
  ll newl = k * l + b;
  if (newm < curm) tmp = curnode->line, curnode->line = {k, b}, k = tmp.first, b = tmp.second;
  bool left = (newl <= curl), mid = (newm <= curm);
  if (left != mid) {
    if (curnode->left == nullptr) {
      curnode->left = new node;
      curnode->left->line = curnode->line;
    }
    add(curnode->left, l, m, k, b);
  } else {
    if (curnode->right == nullptr) {
      curnode->right = new node;
      curnode->right->line = curnode->line;
    }
    add(curnode->right, m + 1, r, k, b);
  }
}

ll get(node* curnode, ll l, ll r, ll x) {
  if (l == r) return curnode->line.first * l + curnode->line.second;
  ll m = (l + r) / 2;
  ll cur = curnode->line.first * x + curnode->line.second, recres = inf_ll;
  if (x <= m && curnode->left != nullptr) recres = get(curnode->left, l, m, x);
  else if (x > m && curnode->right != nullptr) recres = get(curnode->right, m + 1, r, x);
  return min(cur, recres);
}

int main() {
  fast_io;

  ll n, k, i, p;
  ll LB = 0, RB = 1000001, y;
  cin >> n >> k;
  for (i = 1; i <= n; i++) cin >> a[i], dp[i][1] = (a[i] - a[1]) * (a[i] - a[1]);
  node* root = new node;
  for (p = 2; p <= k; p++) {
    refresh(root, LB, RB);
    add(root, LB, RB, -2 * a[p], a[p] * a[p]);
    for (i = p + 1; i <= n; i++) {
      y = get(root, LB, RB, a[i]);
      dp[i][p] = y == inf_ll ? dp[i - 1][p - 1] : min(dp[i - 1][p - 1], y + a[i] * a[i]);
      add(root, LB, RB, -2 * a[i], dp[i - 1][p - 1] + a[i] * a[i]);
      // cout << i << ' ' << p << ' ' << dp[i][p] << ' ' << y << '\n';
    }
  }
  cout << dp[n][k];

  return 0;
}
