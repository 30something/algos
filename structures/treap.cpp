#include "template.h"

struct treap {
  treap(ll nkey, ll nprr, ll nzn) : key(nkey), prr(nprr), sz(1), add_sg(0), zn(nzn) {};
  treap* left = nullptr;
  treap* right = nullptr;
  ll key, prr, sz, add_sg, zn;
};
vector<pll> dbg;
treap* root = nullptr;
void push(treap* first) {
  if (first == nullptr) return;
  if (first->add_sg != 0) {
    if (first->left != nullptr) {
      first->left->add_sg += first->add_sg;
      first->left->key += first->add_sg;
    }
    if (first->right != nullptr) {
      first->right->add_sg += first->add_sg;
      first->right->key += first->add_sg;
    }
    first->add_sg = 0;
  }
}
void recalc(treap* first) {
  if (first == nullptr) return;
  ll left_sz = 0, right_sz = 0;
  if (first->left != nullptr) left_sz = first->left->sz;
  if (first->right != nullptr) right_sz = first->right->sz;
  first->sz = left_sz + right_sz + 1;
}
treap* merge(treap* first, treap* second) {
  if (first == nullptr) return second;
  if (second == nullptr) return first;
  if (first->prr > second->prr) {
    push(first);
    first->right = merge(first->right, second);
    recalc(first);
    return first;
  }
  else {
    push(second);
    second->left = merge(first, second->left);
    recalc(second);
    return second;
  }
}
pair<treap*, treap*> split(treap* first, ll key) {
  if (first == nullptr) return {nullptr, nullptr};
  pair<treap*, treap*> result;
  push(first);
  if (key > first->key) {
    result = split(first->right, key);
    first->right = result.first;
    recalc(result.first);
    recalc(result.second);
    recalc(first);
    return {first, result.second};
  } else {
    result = split(first->left, key);
    first->left = result.second;
    recalc(result.first);
    recalc(result.second);
    recalc(first);
    return {result.first, first};
  }
}
treap* insert(treap* first, ll key, ll prr, ll zn) {
  // 'insert' doesn't work with adding on segments
  if (first == nullptr) return new treap(key, prr, zn);
  if (first->prr < prr) {
    pair<treap*, treap*> pr = split(first, key);
    auto* elem = new treap(key, prr, zn);
    elem->left = pr.first;
    elem->right = pr.second;
    recalc(elem);
    return elem;
  } else {
    treap* result;
    if (key <= first->key) {
      result = insert(first->left, key, prr, zn);
      first->left = result;
    } else {
      result = insert(first->right, key, prr, zn);
      first->right = result;
    }
    recalc(first);
    return first;
  }
}
treap* remove(treap* first, ll key) {
  // 'remove' doesn't work with adding on segments
  if (first == nullptr) return first;
  if (first->key == key) {
    treap* result = merge(first->left, first->right);
    if (root == first) root = result;
    delete first;
    recalc(result);
    return result;
  } else {
    if (key < first->key) first->left = remove(first->left, key);
    else first->right = remove(first->right, key);
    recalc(first);
    return first;
  }
}
void clear(treap* first) {
  if (first == nullptr) return;
  if (first->left != nullptr) clear(first->left);
  if (first->right != nullptr) clear(first->right);
  delete first;
}
void dfs(treap* first) {
  if (first == nullptr) return;
  push(first);
  dbg.eb(first->key, first->prr);
  dfs(first->left);
  dfs(first->right);
}

int main() {
  fast_io;

  clear(root);
  //cerr << '\n' << "Time execute: " << clock() / (double)CLOCKS_PER_SEC << " sec" << '\n';
  return 0;
}
