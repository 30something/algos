#include "template.h"

const ll N = 1e3 + 5;
ll phi[N], mob[N];
ll deg[N], cnt[N];  // power of smallest prime factor (count other mult. functions)
bool comp[N];
vector<ll> prm;

int main() {
  fast_io;

  phi[1] = 1;
  cnt[1] = 1;
  deg[1] = 1;
  mob[1] = 1;
  for (ll i = 2; i < N; i++) {
    if (!comp[i]) {
      prm.pb(i);
      phi[i] = i - 1;
      mob[i] = -1;
      deg[i] = i;
      cnt[i] = 1;
    }
    for (ll j = 0; j < sz(prm) && i * prm[j] < N; j++) {
      comp[i * prm[j]] = true;
      if (i % prm[j] > 0) {
        phi[i * prm[j]] = phi[i] * phi[prm[j]];
        mob[i * prm[j]] = mob[i] * mob[prm[j]];
        deg[i * prm[j]] = prm[j];
        cnt[i * prm[j]] = 1;
      } else {
        // (i != deg[i]) func[i / p^cnt[i]] * func[p^(cnt[i]+1)] = func[i / deg[i]] * func[deg[i] * prm[j]]
        // (i == deg[i]) func[i]
        phi[i * prm[j]] = phi[i] * prm[j];
        mob[i * prm[j]] = 0;
        deg[i * prm[j]] = deg[i] * prm[j];
        cnt[i * prm[j]] = cnt[i] + 1;
        break;
      }
    }
  }

  for (ll i = 1; i < N; i++) {
    cout << i << ' ' << comp[i] << ' ' << phi[i] << ' ' << mob[i] << ' ' << cnt[i] << '\n';
  }

  return 0;
}
