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
