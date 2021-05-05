#include <iostream>
#include <vector>

struct edge {
  int from;
  int to;
  edge(int from, int to) : from(from), to(to) {}
};
void euler_cycle(const std::vector<edge> &edges,
                 std::vector<int> &ptr,
                 const std::vector<std::vector<int>> &g,
                 std::vector<bool> &used,
                 std::vector<edge> &ans,
                 int v) {
  while (ptr[v] != g[v].size()) {
    int e = g[v][ptr[v]];
    if (used[e]) {
      ++ptr[v];
      continue;
    }
    int u = edges[e].to;
    used[e] = true;
    euler_cycle(edges, ptr, g, used, ans, u);
    ans.push_back(edges[e]);
  }
}

int main() {
  int a, k, n;
  std::cin >> n >> a;
  std::vector<edge> edges;
  std::vector<bool> used;
  std::vector<int> ptr(n);
  std::vector<edge> ans;
  std::vector<std::vector<int>> g(n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> k;
      if (k == 0 && i != j) {
        edges.emplace_back(i, j);
        g[i].push_back(edges.size() - 1);
      }
    }
  }
  used.resize(edges.size(), false);
  euler_cycle(edges, ptr, g, used, ans, a - 1);
  for (int i = ans.size() - 1; i >= 0; --i) {
    std::cout << ans[i].from + 1 << ' ' << ans[i].to + 1 << '\n';
  }
  return 0;
}
