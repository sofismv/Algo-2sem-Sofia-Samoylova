#include <iostream>
#include <vector>

void dfs(std::vector<int> &tin,
         std::vector<int> &tout,
         int &timer,
         std::vector<int> &used,
         const std::vector<std::vector<int>> &g,
         int v) {
  tin[v] = timer++;
  used[v] = 1;
  for (int to : g[v]) {
    if (used[to]) {
      continue;
    }
    dfs(tin, tout, timer, used, g, to);
  }
  tout[v] = timer++;
}

bool is_parent(const std::vector<int> &tin, const std::vector<int> &tout, int a, int b) {
  return tout[a] > tout[b] && tin[a] < tin[b];
}

int main() {
  int n, parent, m, a, b, root, timer = 0;
  std::cin >> n;
  std::vector<std::vector<int>> g(n);
  std::vector<int> tin(n);
  std::vector<int> tout(n);
  std::vector<int> used(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> parent;
    if (parent == 0) {
      root = i;
      continue;
    }
    parent--;
    g[parent].push_back(i);
  }
  dfs(tin, tout, timer, used, g, root);
  std::cin >> m;
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b;
    --a;
    --b;
    std::cout << is_parent(tin, tout, a, b) << '\n';
  }
  return 0;
}
