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

const int N = 10000;
vector<int> a[N];
vector<int> b;

int main() {
  fast_io;

  int type;
  cin >> type;

  if (type == 0) {

    int n, m, i, j;
    cin >> n >> m;
    for (i = 0; i < n; i++) {
      a[i].reserve(m);
      for (j = 0; j < m; j++) {
        cin >> a[i][j];
      }
    }

    vector<int> ans;

    // 1st 4 bytes - for n, m

    int n1 = n >> 8, n2 = n & 255;
    int m1 = m >> 8, m2 = m & 255;
    ans.eb(static_cast<uint8_t>(n1));
    ans.eb(static_cast<uint8_t>(n2));
    ans.eb(static_cast<uint8_t>(m1));
    ans.eb(static_cast<uint8_t>(m2));

    // test - RLE

    int pv = 0;
    int cnt = 0;
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        if (i == 0 && j == 0) pv = a[0][0], cnt = 1;
        else {
          if (a[i][j] == pv) {
            if (cnt == 255) ans.eb(255), ans.eb(pv + 128), cnt = 1;
            else cnt++;
          } else ans.eb(cnt), ans.eb(pv + 128), cnt = 1, pv = a[i][j];
        }
      }
    }
    if (cnt > 0) ans.eb(cnt), ans.eb(pv + 128);

    cout << sz(ans) << '\n';
    for (auto f : ans) cout << f << ' ';

  } else {

    int k, i, j;
    cin >> k, b.reserve(k);
    for (i = 0; i < k; i++) cin >> b[i];

    // 1st 4 bytes - for n, m

    ll b0 = b[0], b1 = b[1], b2 = b[2], b3 = b[3];
    ll n = (b0 << 8) + b1;
    ll m = (b2 << 8) + b3;
    cout << n << ' ' << m << '\n';

    // test - RLE

    int ko = 0, kl;
    int vl;
    for (i = 4; i < k; i += 2) {
      kl = b[i];
      vl = b[i + 1];
      for (j = 0; j < kl; j++) {
        cout << vl - 128, ko++;
        if (ko % m == 0) cout << '\n', ko = 0;
        else cout << ' ', ko++;
      }
    }

  }

  return 0;
}
