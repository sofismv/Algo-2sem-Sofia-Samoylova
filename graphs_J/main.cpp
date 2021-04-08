#include <iostream>
#include <vector>

struct edge {
  int from;
  int to;
  edge(int from, int to) : from(from), to(to) {}
};
int n;
std::vector<edge> edges;
std::vector<bool> used;
std::vector<int> ptr;
std::vector<edge> ans;
std::vector<std::vector<int>> g;
void euler_cycle(int v) {
  while (ptr[v] != g[v].size()) {
    int e = g[v][ptr[v]];
    if (used[e]) {
      ++ptr[v];
      continue;
    }
    int u = edges[e].to;
    used[e] = true;
    euler_cycle(u);
    ans.push_back(edges[e]);
  }
}

int main() {
  int a, k;
  std::cin >> n >> a;
  g.resize(n);
  ptr.resize(n);
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
  euler_cycle(a - 1);
  for (int i = ans.size() - 1; i >= 0; --i) {
    std::cout << ans[i].from + 1 << ' ' << ans[i].to + 1 << '\n';
  }
  return 0;
}
