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
    float inp;
    uint8_t* ct;
    cin >> n >> m;
    for (i = 0; i < n; i++) {
      a[i].reserve(4 * m);
      for (j = 0; j < 4 * m; j += 4) {
        cin >> inp;
        ct = (uint8_t*)(&inp);
        a[i][j] = ct[0];
        a[i][j + 1] = ct[1];
        a[i][j + 2] = ct[2];
        a[i][j + 3] = ct[3];
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

    // test - LZW

    unordered_map<string, int> mdct;  // two bytes for dict size and dict pos (max size - 65536)
    int lim_size = 65536;
    string tmp;
    char c;
    mdct[""] = 0;
    for (i = 0; i < 256; i++) {
      c = (char)i;
      tmp = "", tmp += c;
      mdct[tmp] = i + 1;
    }
//    for (auto f : mdct) cout << f.first << ' ' << f.second << '\n';

    string cur;
    int lpos = 0;
    for (i = 0; i < n; i++) {
      for (j = 0; j < 4 * m; j++) {
        cur += (char)a[i][j];
        if (!mdct.contains(cur)) {
          ans.eb(lpos >> 8);
          ans.eb(lpos & 255);
          if (sz(mdct) < lim_size) mdct[cur] = sz(mdct);
          cur = "";
          cur += (char)a[i][j];
        }
        lpos = mdct[cur];
      }
    }
    if (!cur.empty()) {
      ans.eb(lpos >> 8);
      ans.eb(lpos & 255);
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

    // test - LZW

    vector<vector<uint8_t>> dct;  // two bytes for dict size and dict pos (max size - 65536)
    dct.pb({});
    for (i = 0; i < 256; i++) {
      dct.pb({(uint8_t)i});
    }

    int bp, lp = (b[4] << 8) + b[5], ko = 0, rm = 0;
    uint8_t ct[4];
    dct.eb(dct[lp]);
    for (i = 6; i < k; i += 2) {
      bp = (b[i] << 8) + b[i + 1];
      dct.back().push_back(dct[bp][0]);
      for (j = 0; j < sz(dct[lp]); j++) {
        ct[rm++] = dct[lp][j];
        if (rm == 4) {
          cout << fixed << setprecision(10) << *(float*)(&ct), ko++;
          if (ko % m == 0) cout << '\n', ko = 0;
          else cout << ' ';
          rm = 0;
        }
      }
      dct.eb(dct[bp]);
      lp = bp;
    }
    for (j = 0; j < sz(dct[lp]); j++) {
      ct[rm++] = dct[lp][j];
      if (rm == 4) {
        cout << fixed << setprecision(10) << *(float*)(&ct), ko++;
        if (ko % m == 0) cout << '\n', ko = 0;
        else cout << ' ';
        rm = 0;
      }
    }

  }

  return 0;
}
