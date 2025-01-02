#include <bits/stdc++.h>
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>

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

//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,abm,avx,avx2,fma,bmi,bmi2,lzcnt,popcnt")
//#pragma GCC optimize("Ofast", "unroll-loops")
//#pragma GCC optimize("O3")

using namespace std;
//using namespace __gnu_pbds;
//typedef tree<ll, null_type, less_equal<>, rb_tree_tag,
//    tree_order_statistics_node_update> ordered_set;

ld eps = 1e-9;
ll inf_ll = 1e18 + 100, mod1 = 1e9 + 7, mod2 = 998244353;
mt19937 gen32(chrono::steady_clock::now().time_since_epoch().count());

ll sqr(ll a) { return a * a; }
ll gcd(ll a, ll b) { return !a ? b : gcd(b % a, a); }
ll binpow(ll a, ll b, ll mod) {
  return b ? (b % 2 ? (a * (sqr(binpow(a, b / 2, mod)) % mod)) % mod :
              sqr(binpow(a, b / 2, mod)) % mod) : 1;
}

ll a[100001];
ll bl[320], res[320];
ll sind[320], lind[320];

int main() {
  fast_io;

  ll n, q, i, j, x, l, r, srn, cg, cb;
  ll bll, blr, ans;
  ll old_mul, new_mul;
  cin >> n;
  srn = sqrt(n);
  cg = cb = 0;
  for (i = 1; i <= n; i++) {
    cin >> a[i];
    if (i > cg) cg += srn, cb++, sind[cb] = i, lind[cb - 1] = i - 1;
    if (res[cb] == 0) res[cb] = 1;
    res[cb] = (res[cb] * a[i]) % mod1;
    bl[i] = cb;
  }
  lind[cb] = n;
  cin >> q;
  while (q--) {
    cin >> x >> l >> r;
    bll = bl[l], blr = bl[r];
    if (x == 0) {

      for (i = bll + 1; i < blr; i++) {
        ans = 1;
        for (j = sind[i]; j <= lind[i]; j++) {
          a[j]++;
          if (a[j] >= mod1) a[j] -= mod1;
          ans = (ans * a[j]) % mod1;
        }
        res[i] = ans;
      }

      if (bll == blr) {

        old_mul = 1, new_mul = 1;
        for (i = l; i <= r; i++) {
          old_mul = (old_mul * a[i]) % mod1;
          a[i]++;
          if (a[i] >= mod1) a[i] -= mod1;
          new_mul = (new_mul * a[i]) % mod1;
        }
        // res[bll] = res[bll] / old_mul
        res[bll] = (res[bll] * binpow(old_mul, mod1 - 2, mod1)) % mod1;
        res[bll] = (res[bll] * new_mul) % mod1;

      } else {

        old_mul = 1, new_mul = 1;
        for (i = l; i <= lind[bll]; i++) {
          old_mul = (old_mul * a[i]) % mod1;
          a[i]++;
          if (a[i] >= mod1) a[i] -= mod1;
          new_mul = (new_mul * a[i]) % mod1;
        }
        // res[bll] = res[bll] / old_mul
        res[bll] = (res[bll] * binpow(old_mul, mod1 - 2, mod1)) % mod1;
        res[bll] = (res[bll] * new_mul) % mod1;

        old_mul = 1, new_mul = 1;
        for (i = sind[blr]; i <= r; i++) {
          old_mul = (old_mul * a[i]) % mod1;
          a[i]++;
          if (a[i] >= mod1) a[i] -= mod1;
          new_mul = (new_mul * a[i]) % mod1;
        }
        // res[blr] = res[blr] / old_mul
        res[blr] = (res[blr] * binpow(old_mul, mod1 - 2, mod1)) % mod1;
        res[blr] = (res[blr] * new_mul) % mod1;

      }

    } else {
      ans = 1;
      for (i = bll + 1; i < blr; i++) ans = (ans * res[i]) % mod1;
      if (bll == blr) {
        for (i = l; i <= r; i++) ans = (ans * a[i]) % mod1;
      } else {
        for (i = l; i <= lind[bll]; i++) ans = (ans * a[i]) % mod1;
        for (i = sind[blr]; i <= r; i++) ans = (ans * a[i]) % mod1;
      }
      cout << ans << '\n';
    }
  }

  return 0;
}
