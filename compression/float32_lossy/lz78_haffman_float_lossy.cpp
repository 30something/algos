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

const int N = 10000000;
vector<float> a;
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

    int n, m, i;
    float eps;
    cin >> n >> m >> eps;
    float real_eps = 0.99f * eps;
    a.resize(n * m);
    map<float, vector<int>> ucnt;
    for (i = 0; i < n * m; i++) {
      cin >> a[i];
      ucnt[a[i]].pb(i);
    }

    vector<pair<ll, float>> vcf;
    for (const auto& f : ucnt) vcf.eb(sz(f.second), f.first);
    sort(rall(vcf));
    for (auto pt : vcf) {
      if (!ucnt.contains(pt.second)) continue;
      auto f = ucnt.lower_bound(pt.second);
      auto g = f;
      vector<float> rm;
      while (f != --ucnt.end()) {
        f++;
        if (f->first - pt.second <= real_eps) {
          for (auto t : f->second) a[t] = pt.second;
          rm.eb(f->first);
        } else break;
      }
      while (g != ucnt.begin()) {
        g--;
        if (pt.second - g->first <= real_eps) {
          for (auto t : g->second) a[t] = pt.second;
          rm.eb(g->first);
        } else break;
      }
      for (auto r : rm) ucnt.erase(r);
    }

    assert(m == 2);

    vector<int> ans;

    // 1st 3 bytes - for n

    int tn = n;
    for (i = 0; i < 3; i++, tn >>= 8) {
      ans.eb(static_cast<uint8_t>(tn & 255));
    }

    // LZ78

    map<vector<float>, int> mdct;  // two bytes for dict size and dict pos (max size - 32767)
    vector<float> tmp;
    int lim_size = 32767;
    mdct[{}] = 0;

    int lpos = 0;
    uint8_t* ct;
    for (i = 0; i < n * m; i++) {
      tmp.eb(a[i]);
      if (!mdct.contains(tmp)) {
        if (lpos <= 127) ans.eb(lpos);
        else {
          ans.eb((lpos >> 8) + 128);
          ans.eb(lpos & 255);
        }
        ct = (uint8_t*)(&a[i]);
        ans.eb(ct[0]);
        ans.eb(ct[1]);
        ans.eb(ct[2]);
        ans.eb(ct[3]);
        if (sz(mdct) < lim_size) mdct[tmp] = sz(mdct);
        tmp.clear();
        lpos = 0;
      } else {
        if (i < n * m - 1) lpos = mdct[tmp];
        else {
          if (lpos <= 127) ans.eb(lpos);
          else {
            ans.eb((lpos >> 8) + 128);
            ans.eb(lpos & 255);
          }
          ct = (uint8_t*)(&a[n * m - 1]);
          ans.eb(ct[0]);
          ans.eb(ct[1]);
          ans.eb(ct[2]);
          ans.eb(ct[3]);
        }
      }
    }

    // Haffmann

    vector<int> vans;
    vans.pb(ans[0]);
    vans.pb(ans[1]);
    vans.pb(ans[2]);

    int enc_amount = sz(ans) - 3;
    vector<int> tv;
    while (enc_amount > 0) tv.eb(enc_amount & 255), enc_amount >>= 8;
    vans.eb(sz(tv));
    reverse(all(tv));
    for (auto f : tv) vans.eb(f);

    unordered_map<int, int> cnt;
    multiset<pii, cmpr> haff;
    unordered_map<int, pair<int, bool>> haff_link;
    for (i = 3; i < sz(ans); i++) cnt[ans[i]]++;
    for (auto f : cnt) haff.insert({f.second, f.first});

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

    // struct: (num of values) - (val - code size in bits - code)
    // coding table

    vans.eb(sz(cnt) - 1);

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
      vans.eb(f.first);
      vans.eb(szb);
      for (int k = 0; k < (szb + 7) / 8; k++) {
        uint8_t outp = 0;
        for (int k8 = 0; k8 < 8; k8++) {
          outp <<= 1, outp |= code.back(), code.pop_back();
          if (code.empty()) {
            outp <<= (7 - k8);
            break;
          }
        }
        vans.eb(outp);
      }
    }

    // encoded seq

    vector<bool> pcode;
    int bits_used = 0, left_val = 0;
    for (i = 3; i < sz(ans); i++) {
      pcode = haff_code[ans[i]];
      for (int k = sz(pcode) - 1; k >= 0; k--) {
        left_val <<= 1;
        left_val |= pcode[k];
        bits_used++;
        if (bits_used == 8) vans.eb(left_val), bits_used = left_val = 0;
      }
    }
    if (bits_used > 0) left_val <<= (8 - bits_used), vans.pb(left_val);

    cout << sz(vans) << '\n';
    for (auto f : vans) cout << f << ' ';

  } else {

    int k, i, j;
    cin >> k, b.reserve(k);
    for (i = 0; i < k; i++) cin >> b[i];

    // 1st 3 bytes - for n

    int n = 0;
    for (i = 2; i >= 0; i--) {
      n <<= 8;
      n |= b[i];
    }
    cout << n << ' ' << 2 << '\n';

    // Haffmann

    // create coding table

    int enc_amount_sz_byte = b[3];
    int enc_amount_sz = 0;
    for (i = 4; i < 4 + enc_amount_sz_byte; i++) {
      enc_amount_sz = (enc_amount_sz << 8) | b[i];
    }

    int vocab_values_sz = b[4 + enc_amount_sz_byte] + 1;

    int bitsz, bytesz, zn;
    unordered_map<string, int> dcthaff;
    bitset<8> btvw;
    for (i = 5 + enc_amount_sz_byte; i < k; ) {
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
      dcthaff[tmp] = zn;
      i += bytesz + 2;
      if (sz(dcthaff) == vocab_values_sz) break;
    }

    // decode

    vector<int> vans;

    int elements_decoded = 0;
    string tmp;
    for ( ; i < k; i++) {
      btvw = b[i];
      for (j = 0; j < 8; j++) {
        tmp += to_string(btvw[7 - j]);
        if (dcthaff.contains(tmp)) {
          vans.eb(dcthaff[tmp]);
          elements_decoded++;
          tmp = "";
        }
        if (elements_decoded == enc_amount_sz) break;
      }
      if (elements_decoded == enc_amount_sz) break;
    }

    // LZ78

    vector<vector<float>> dct;  // two bytes for dict size and dict pos (max size - 32767)
    dct.pb({});

    int bp, ko = 0, m = 2;
    uint8_t ct[4];
    float val;
    for (i = 0; i < sz(vans); ) {
      bp = vans[i];
      if (bp <= 127) {
        ct[0] = vans[i + 1];
        ct[1] = vans[i + 2];
        ct[2] = vans[i + 3];
        ct[3] = vans[i + 4];
        i += 5;
      } else {
        bp = (vans[i] << 8) + vans[i + 1];
        bp &= 32767;
        ct[0] = vans[i + 2];
        ct[1] = vans[i + 3];
        ct[2] = vans[i + 4];
        ct[3] = vans[i + 5];
        i += 6;
      }
      val = *(float*)(&ct);
      for (j = 0; j < sz(dct[bp]); j++) {
        cout << fixed << setprecision(20) << dct[bp][j], ko++;
        if (ko % m == 0) cout << '\n', ko = 0;
        else cout << ' ';
      }
      cout << fixed << setprecision(20) << val, ko++;
      if (ko % m == 0) cout << '\n', ko = 0;
      else cout << ' ';
      dct.eb(dct[bp]);
      dct.back().push_back(val);
    }

  }

  return 0;
}
