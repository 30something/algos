#include "template.h"

vector<pair<pii, int>> vc, nvc;
vector<pii> cnt[400010];
int cls[400010], lcp[400010];
void digital_sort(vector<pair<pii, int>> &v) {
  int i, n = v.size();
  vector<pair<pii, int>> vct;
  for (i = 0; i < n; i++) cnt[i].clear();
  for (i = 0; i < n; i++) cnt[v[i].first.first].pb({v[i].first.second, v[i].second});
  for (i = 0; i < n; i++)
    for (auto f: cnt[i])
      vct.pb({{i, f.first}, f.second});
  v = vct;
}

int main() {
  fast_io;
  int n, i, j, k, pos;
  string a;
  cin >> a;
  a += "$";
  n = a.size();
  for (i = 0; i < n; i++) vc.pb({{a[i], a[i]}, i});
  sort(all(vc));
  cls[vc[0].second] = 0;
  for (i = 1; i < n; i++) {
    if (vc[i].first == vc[i - 1].first) cls[vc[i].second] = cls[vc[i - 1].second];
    else cls[vc[i].second] = cls[vc[i - 1].second] + 1;
  }
  for (k = 1; k <= n; k *= 2) {
    nvc.clear();
    for (i = 0; i < n; i++)
      nvc.pb({{cls[(vc[i].second - k + n) % n], cls[vc[i].second]}, (vc[i].second - k + n) % n});
    vc = nvc;
    digital_sort(vc);
    cls[vc[0].second] = 0;
    for (i = 1; i < n; i++) {
      if (vc[i].first == vc[i - 1].first) cls[vc[i].second] = cls[vc[i - 1].second];
      else cls[vc[i].second] = cls[vc[i - 1].second] + 1;
    }
  }
  for (i = 0, k = 0; i < n - 1; i++) {
    pos = cls[i];
    j = vc[pos - 1].second;
    while (a[i + k] == a[j + k]) k++;
    lcp[pos] = k;
    k = max(k - 1, 0);
  }
  for (i = 0; i < n; i++) cout << vc[i].second << ' ';
  cout << '\n';
  for (i = 1; i < n; i++) cout << lcp[i] << ' ';
  return 0;
}
