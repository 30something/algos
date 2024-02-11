#include "template.h"

int phi(int n) {
  int res = n;
  for (int i = 2; i * i <= n; i++) {
    if (n % i) continue;
    while (n % i == 0) n /= i;
    res -= res / i;
  }
  if (n > 1) res -= res / n;
  return res;
}

int n_th_primitive_root(int n) {

  // algo:
  // 1) factorize phi(n) = p_1^(a_1) * ... * (p_c)^(a_c)
  // if n = 998244353 = 2^23 * 7 * 17 + 1 then p_1 = 2, p_2 = 7, p_3 = 17
  // 2) bruteforce g and check that g^(phi(n) / p_i) NOT equal to 1 modulo n for all p_i
  // 3) if 2) is right then g - primitive root
  // upper bound for g - O(log^6 n)

  int ph = phi(n), tph;
  tph = ph;
  vector<int> p;
  for (int i = 2; i * i <= ph; i++) {
    if (ph % i) continue;
    while (ph % i == 0) ph /= i;
    p.pb(i);
  }
  if (ph > 1) p.pb(ph);
  vector<int> d;
  for (auto pr : p) d.pb(tph / pr);
  for (int g = 2; g < n; g++) {
    bool check = true;
    for (auto dv : d) {
      if (binpow(g, dv, n) == 1) {
        check = false;
        break;
      }
    }
    if (check) return g;
  }
  exit(-1);
}

ll g_powers_mod2[21];  // g_powers_mod2[i] = g^(2^i) mod 998244353

void fft(vector<int>& a, ll deg) {
  int k = sz(a), nk = (k >> 1);
  if (k == 1) return;
  vector<int> even(nk), odd(nk);
  for (int i = 0; i < nk; i++) {
    even[i] = a[i << 1];
    odd[i] = a[(i << 1) | 1];
  }
  fft(even, deg + 1);
  fft(odd, deg + 1);
  ll tg = 1, g = g_powers_mod2[deg];
  int res;
  for (int i = 0; i < nk; i++) {
    res = (tg * odd[i]) % mod2;
    a[i] = even[i] + res;
    if (a[i] >= mod2) a[i] -= mod2;
    a[i + nk] = even[i] - res;
    if (a[i + nk] < 0) a[i + nk] += mod2;
    tg = (tg * g) % mod2;
  }
}

int main() {
  fast_io;
  ll n, m, i, k = 1, t = 0;
  cin >> n >> m;
  while (k < n + m) k <<= 1, t++;

  vector<int> a(k), b(k);
  for (i = 0; i < n; i++) cin >> a[i];
  for (i = 0; i < m; i++) cin >> b[i];

  // calculate convolution modulo 998244353
  // z_j = e ^ ((2pi * i * j) / k) - version with complex values

  // finding n-th primitive root g (works in O(ans * log(phi(n)) * log n))
  // can be done separately from convolution
  // g - primitive root modulo n if all g^t (t = 0 .. n-1) are different modulo n
  // g - primitive root of order d modulo n if all g^t (t = 0 .. d-1) are different modulo n
  // (existence: (n-1) is divisible by d)
  // so, here we can use length up to 2^23

  ll g = n_th_primitive_root(998244353);

  // g - is primitive root, but additional condition is g^k = 1 (mod n)
  // so if n - 1 = 998244352 = 2^23 * 7 * 17 then g^(2^23 * 7 * 17) = 1 (mod 998244353) (fermat's little theorem)
  // and (g^(2^(23 - t) * 7 * 17))^(2^t) = 1 (mod n) where 2^t = k

//    cout << g << ' ' << t << '\n';
  g = binpow(g, (1 << (23 - t)) * 119, mod2);
//    cout << (1 << (23 - t)) * 119 << '\n';

  g_powers_mod2[0] = g;
  for (i = 1; i <= 20; i++) {
    g_powers_mod2[i] = (g_powers_mod2[i - 1] * g_powers_mod2[i - 1]) % mod2;
  }

  vector<int> c(k);
  fft(a, 0);
  fft(b, 0);
  for (i = 0; i < k; i++) c[i] = ((ll)a[i] * b[i]) % mod2;

  // inverse fft
  fft(c, 0);
  ll inv_k = binpow(k, mod2 - 2, mod2);
  for (auto& f : c) f = ((ll)f * inv_k) % mod2;
  reverse(c.begin() + 1, c.end());

  ll cnt = n + m - 1;
  for (auto f : c) {
    cout << f << ' ';
    if (--cnt == 0) break;
  }

  return 0;
}
