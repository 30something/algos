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
vector<bool> haff_code[256];

struct cmpr {
  bool operator()(const pii& x, const pii& y) const {
    return x.first != y.first ? x.first < y.first : x.second > y.second;
  }
};

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

    // test - Haffman

    unordered_map<int, int> cnt;
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        cnt[a[i][j] + 128]++;
      }
    }
    multiset<pii, cmpr> haff;
    for (auto f : cnt) haff.insert({f.second, f.first});
    unordered_map<int, pair<int, bool>> haff_link;
    int link_val = 256;
    if (sz(haff) == 1) haff_link[haff.begin()->second] = {link_val, 1}, link_val++;
    while (sz(haff) > 1) {
      pii p1 = {haff.begin()->first, haff.begin()->second};
      haff.erase(haff.begin());
      pii p2 = {haff.begin()->first, haff.begin()->second};
      haff.erase(haff.begin());
      haff.insert({p1.first + p2.first, link_val});
      haff_link[p1.second] = {link_val, 0};
      haff_link[p2.second] = {link_val, 1};
      link_val++;
    }

    // struct: (num of values) - (val - code size in bits - code) (max - (1) + (1 + 1 + x) per val, x <= 32)
    // coding table

    ans.eb(sz(cnt) - 1);

    int val, szb;
    vector<bool> code;
    for (auto f : cnt) {
      val = f.first;
      szb = 0;
      code = {};
      // cout << val << " : ";
      while (val < link_val - 1) {
        pii nxt = haff_link[val];
        // cout << nxt.second << ' ';
        val = nxt.first;
        szb++;
        code.pb(nxt.second);
      }
      haff_code[f.first] = code;
      // cout << " * " << szb << '\n';
      ans.eb(f.first);
      ans.eb(szb);
      for (int k = 0; k < (szb + 7) / 8; k++) {
        uint8_t outp = 0;
        for (int k8 = 0; k8 < 8; k8++) {
          outp <<= 1, outp |= code.back(), code.pop_back();
          if (code.empty()) {
            outp <<= (7 - k8);
            break;
          }
        }
        ans.eb(outp);
      }
    }

    // encoded seq

    vector<bool> pcode;
    int bits_used = 0, left_val = 0;
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        pcode = haff_code[a[i][j] + 128];
        for (int k = sz(pcode) - 1; k >= 0; k--) {
          left_val <<= 1;
          left_val |= pcode[k];
          bits_used++;
          if (bits_used == 8) ans.eb(left_val), bits_used = left_val = 0;
        }
      }
    }
    if (bits_used > 0) left_val <<= (8 - bits_used), ans.pb(left_val);

    cout << sz(ans) << '\n';
    for (auto f : ans) cout << f << ' ';

  } else {

    int k, i, j;
    cin >> k, b.reserve(k);
    for (i = 0; i < k; i++) cin >> b[i];

    // 1st 4 bytes - for n, m

    int b0 = b[0], b1 = b[1], b2 = b[2], b3 = b[3];
    int n = (b0 << 8) + b1;
    int m = (b2 << 8) + b3;
    int nm = n * m;
    cout << n << ' ' << m << '\n';

    // test - Haffman

    // create coding table

    int vocab_values_sz = b[4] + 1;

    int bitsz, bytesz, zn;
    unordered_map<string, int> dct;
    bitset<8> btvw;
    for (i = 5; i < k; ) {
      zn = b[i];
      bitsz = b[i + 1];
      bytesz = (bitsz + 7) / 8;
      string tmp;
      int ovl = 0;
      for (j = 0; j < bytesz; j++) {
        btvw = b[i + 2 + j];
        for (int bts = 0; bts < 8; bts++) {
          tmp += to_string(btvw[7 - bts]);
          ovl++;
          if (ovl >= bitsz) break;
        }
        if (ovl >= bitsz) break;
      }
      dct[tmp] = zn;
      i += bytesz + 2;
      if (sz(dct) == vocab_values_sz) break;
    }

    // decode

    int elements_decoded = 0;
    string tmp;
    for ( ; i < k; i++) {
      btvw = b[i];
      for (j = 0; j < 8; j++) {
        tmp += to_string(btvw[7 - j]);
        if (dct.contains(tmp)) {
          cout << dct[tmp] - 128;
          elements_decoded++;
          tmp = "";
          if (elements_decoded % m == 0) cout << '\n';
          else cout << ' ';
        }
        if (elements_decoded == nm) break;
      }
      if (elements_decoded == nm) break;
    }

  }

  return 0;
}
