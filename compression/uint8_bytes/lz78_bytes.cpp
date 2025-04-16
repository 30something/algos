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

    // test - LZ78

    unordered_map<string, int> mdct;  // two bytes for dict size and dict pos (max size - 65536)
    int lim_size = 65536;
    mdct[""] = 0;

    string cur;
    int lpos = 0;
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        cur += (char)(a[i][j] + 128);
//        for (auto f : mdct) cout << f.first << '|' << f.second << ' ';
//        cout << '\n';
//        cout << lpos << ' ' << cur << ' ' << sz(cur) << '\n';
        if (!mdct.contains(cur)) {
          ans.eb(lpos >> 8);
          ans.eb(lpos & 255);
          ans.eb(a[i][j] + 128);
          if (sz(mdct) < lim_size) mdct[cur] = sz(mdct);
          cur = "";
          lpos = 0;
        } else {
          if (i < n - 1 || j < m - 1) lpos = mdct[cur];
          else {
            ans.eb(lpos >> 8);
            ans.eb(lpos & 255);
            ans.eb(a[n - 1][m - 1] + 128);
          }
        }
      }
    }

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

    // test - LZ78

    vector<string> dct;  // two bytes for dict size and dict pos (max size - 65536)
    dct.eb("");

    int bp, val, ko = 0;
    for (i = 4; i < k; i += 3) {
      bp = (b[i] << 8) + b[i + 1];
      val = b[i + 2] - 128;
      for (j = 0; j < sz(dct[bp]); j++) {
        cout << (int)dct[bp][j], ko++;
        if (ko % m == 0) cout << '\n', ko = 0;
        else cout << ' ', ko++;
      }
      cout << val, ko++;
      if (ko % m == 0) cout << '\n', ko = 0;
      else cout << ' ', ko++;
      dct.eb(dct[bp] + (char)(b[i + 2] - 128));
//      cout << "***" << bp << ' ' << (int)dct[bp][0] << '\n';
    }

  }

  return 0;
}
