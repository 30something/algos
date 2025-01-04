#include "template.h"

const ll N = 1e6 + 5;
ll phi[N], mob[N];
ll deg[N], cnt[N];  // power of smallest prime factor (count other mult. functions)
bool comp[N];
vector<ll> prm;

ll check1(ll n) {
  ll ans = 0, i, j;
  for (i = 2; i <= n; i++) {
    for (j = i + 1; j <= n; j++) {
      if (gcd(i, j) == 1) ans += 2;
    }
  }
  return 2 * n - 1 + ans;
}

ll check2(ll n) {
  ll ans = n * (n + 1) / 2, i, j;
  for (i = 1; i <= n; i++) {
    for (j = i + 1; j <= n; j++) {
      ans += 2 * gcd(i, j);
    }
  }
  return ans;
}

ll f3[N];  // f3 is mult and f3(p^k) = p^k - p^(k+1)
ll check3(ll n) {
  ll ans = n * (n + 1) / 2, i, j;
  for (i = 1; i <= n; i++) {
    for (j = i + 1; j <= n; j++) {
      ans += 2 * i * j / gcd(i, j);
    }
  }
  return ans;
}

ld f4[N];  // f4 is mult and f4(p^k) = p^(-k) - p^(1-k)
ll check4(const vector<ll>& a) {
  ll ans = 0;
  for (auto f : a) {
    for (auto g : a) {
      ans += f * g / gcd(f, g);
    }
  }
  return ans;
}

int main() {
  fast_io;

  phi[1] = 1;
  cnt[1] = 1;
  deg[1] = 1;
  mob[1] = 1;
  f3[1] = 1;
  f4[1] = 1;
  for (ll i = 2; i < N; i++) {
    if (!comp[i]) {
      prm.pb(i);
      phi[i] = i - 1;
      mob[i] = -1;
      deg[i] = i;
      cnt[i] = 1;
      f3[i] = i - i * i;
      f4[i] = 1. / i - 1;
    }
    for (ll j = 0; j < sz(prm) && i * prm[j] < N; j++) {
      comp[i * prm[j]] = true;
      if (i % prm[j] > 0) {
        phi[i * prm[j]] = phi[i] * phi[prm[j]];
        mob[i * prm[j]] = mob[i] * mob[prm[j]];
        deg[i * prm[j]] = prm[j];
        cnt[i * prm[j]] = 1;
        f3[i * prm[j]] = f3[i] * f3[prm[j]];
        f4[i * prm[j]] = f4[i] * f4[prm[j]];
      } else {
        // (i != deg[i]) func[i / p^cnt[i]] * func[p^(cnt[i]+1)] = func[i / deg[i]] * func[deg[i] * prm[j]]
        // (i == deg[i]) func[i]
        phi[i * prm[j]] = phi[i] * prm[j];
        mob[i * prm[j]] = 0;
        deg[i * prm[j]] = deg[i] * prm[j];
        cnt[i * prm[j]] = cnt[i] + 1;
        f3[i * prm[j]] = f3[i / deg[i]] * deg[i] * prm[j] * (1 - prm[j]);
        f4[i * prm[j]] = f4[i / deg[i]] * (1. / deg[i]) * (1. / prm[j] - 1);
        break;
      }
    }
  }

//  for (ll i = 1; i < N; i++) {
//    cout << i << ' ' << comp[i] << ' ' << phi[i] << ' ' << mob[i] << ' ' << cnt[i] << '\n';
//  }

  ll n, i, j, t;
  cin >> n;

  // Task 1: count number of pairs of coprimes on [1; n]
  ll ans1 = 0, gans1 = check1(n);
  for (i = 1; i <= n; i++) {
    ans1 += mob[i] * (n / i) * (n / i);
  }
  cout << ans1 << ' ' << gans1 << ' ' << (ans1 == gans1) << '\n';

  // Task 2: count sum of gcd(i, j) up to n
  ll ans2 = 0, altans2 = 0, gans2 = check2(n);
  for (i = 1; i <= n; i++) {
    ll tans2 = 0;
    for (j = 1; j <= n / i; j++) {
      t = n / (i * j);
      tans2 += mob[j] * t * t;
    }
    ans2 += i * tans2;
    altans2 += phi[i] * (n / i) * (n / i);
  }
  cout << ans2 << ' ' << altans2 << ' ' << gans2 << ' ' << (ans2 == gans2) << ' ' << (altans2 == gans2) << '\n';

  // Task 3: count sum of lcm(i, j) up to n
  ll ans3 = 0, altans3 = 0, gans3 = check3(n);
  for (i = 1; i <= n; i++) {
    ll tans3 = 0;
    for (j = 1; j <= n / i; j++) {
      t = n / (i * j);
      tans3 += mob[j] * j * j * ((t * (t + 1)) * (t * (t + 1)) / 4);
    }
    ans3 += i * tans3;
    t = n / i; altans3 += f3[i] * ((t * (t + 1)) * (t * (t + 1)) / 4);
  }
  cout << ans3 << ' ' << altans3 << ' ' << gans3 << ' ' << (ans3 == gans3) << ' ' << (altans3 == gans3) << '\n';

  // Task 4: count sum of lcm(a[i], a[j]) for array A
  vector<ll> a(n);
  unordered_map<ll, ll> cnta;
  ll mxa = 0;
  for (i = 0; i < n; i++) cin >> a[i], cnta[a[i]]++, mxa = max(mxa, a[i]);
  ld ans4 = 0; ll gans4 = check4(a);
  for (i = 1; i <= n * mxa; i++) {
    ll ta = 0;
    for (j = 1; j <= n * mxa / i; j++) ta += cnta[i * j] * i * j;
    ans4 += f4[i] * ta * ta;
  }
  cout << llroundl(ans4) << ' ' << gans4 << ' ' << (llroundl(ans4) == gans4) << '\n';

  return 0;
}
