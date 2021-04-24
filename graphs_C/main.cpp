#include <iostream>
#include <vector>
#include <algorithm>

enum class colour { WHITE, GRAY, BLACK };

bool dfs(std::vector<int> &tin,
         std::vector<int> &tout,
         std::vector<colour> &color,
         int &timer,
         int &cycle_vertex,
         std::vector<int> &parent,
         const std::vector<std::vector<int>> &g,
         int v) {
  tin[v] = timer++;
  color[v] = colour::GRAY;
  for (int to : g[v]) {
    if (color[to] == colour::WHITE) {
      parent[to] = v;
      if (dfs(tin, tout, color, timer, cycle_vertex, parent, g, to)) {
        return true;
      }
    }
    if (color[to] == colour::GRAY) {
      parent[to] = v;
      cycle_vertex = v;
      return true;
    }
  }
  tout[v] = timer++;
  color[v] = colour::BLACK;
  return false;
}

void print_cycle(const std::vector<int> &parent, int cycle_vertex) {
  std::vector<int> ans;
  int vertex = parent[cycle_vertex];
  while (vertex != cycle_vertex) {
    ans.push_back(vertex + 1);
    vertex = parent[vertex];
  }
  ans.push_back(cycle_vertex + 1);
  std::reverse(ans.begin(), ans.end());
  for (auto i : ans) {
    std::cout << i << ' ';
  }
}

int main() {
  int m, u, v, n, cycle_vertex, timer = 0;
  std::cin >> n >> m;
  std::vector<std::vector<int>> g(n);
  std::vector<int> tin(n);
  std::vector<int> tout(n);
  std::vector<int> parent(n);
  std::vector<colour> color(n, colour::WHITE);
  for (int i = 0; i < m; ++i) {
    std::cin >> u >> v;
    g[--u].push_back(--v);
  }
  for (int v = 0; v < n; ++v) {
    if (color[v] == colour::WHITE) {
      if (dfs(tin, tout, color, timer, cycle_vertex, parent, g, v)) {
        std::cout << "YES\n";
        print_cycle(parent, cycle_vertex);
        return 0;
      }
    }
  }
  std::cout << "NO";
  return 0;
}
