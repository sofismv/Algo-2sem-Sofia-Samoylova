#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

std::vector<std::vector<int>> g;
std::vector<int> tin;
std::vector<int> tout;
std::vector<int> ret;
std::set<int> ans;
int n;
int timer = 0;
std::vector<int> used;

void dfs(int v, int p = -1) {
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
      dfs(to, v);
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
  int m, u, v;
  std::cin >> n >> m;
  g.resize(n);
  tin.resize(n);
  tout.resize(n);
  used.resize(n);
  ret.resize(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> u >> v;
    u--;
    v--;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  for (int i = 0; i < n; ++i) {
    if (used[i] == 0) {
      dfs(i);
    }
  }
  std::cout << ans.size() << '\n';
  for (auto i : ans) {
    std::cout << i + 1 << ' ';
  }
  return 0;
}
