#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

void dfs(std::vector<int> &tin,
         std::vector<int> &tout,
         int &timer,
         std::vector<int> &used,
         std::vector<int> &ret,
         const std::vector<std::vector<int>> &g,
         std::set<int> &ans,
         int v,
         int p = -1) {
  tin[v] = timer++;
  used[v] = 1;
  ret[v] = tin[v];
  int children = 0;
  for (int to : g[v]) {
    if (to == p) {
      continue;
    }
    if (used[to]) {
      ret[v] = std::min(ret[v], tin[to]);
    } else {
      dfs(tin, tout, timer, used, ret, g, ans, to, v);
      ret[v] = std::min(ret[v], ret[to]);
      if (ret[to] >= tin[v] && p != -1) {
        ans.insert(v);
      }
      children++;
    }
  }
  if (p == -1 && children >= 2) {
    ans.insert(v);
  }
}

int main() {
  int m, u, v, n, timer = 0;
  std::cin >> n >> m;
  std::vector<std::vector<int>> g(n);
  std::vector<int> tin(n);
  std::vector<int> tout(n);
  std::vector<int> ret(n);
  std::vector<int> used(n);
  std::set<int> ans;
  for (int i = 0; i < m; ++i) {
    std::cin >> u >> v;
    g[--u].push_back(--v);
    g[v].push_back(u);
  }
  for (int i = 0; i < n; ++i) {
    if (used[i] == 0) {
      dfs(tin, tout, timer, used, ret, g, ans, i);
    }
  }
  std::cout << ans.size() << '\n';
  for (auto i : ans) {
    std::cout << i + 1 << ' ';
  }
  return 0;
}
