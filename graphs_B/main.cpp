#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> g;
std::vector<int> tin;
std::vector<int> tout;
std::vector<int> parent;
int n;
int cycle_vertex;
int timer = 0;
std::vector<std::string> color;

bool dfs(int v) {
  tin[v] = timer++;
  color[v] = "GRAY";
  for (int to : g[v]) {
    if (color[to] == "WHITE") {
      parent[to] = v;
      if (dfs(to)) {
        return true;
      }
    }
    if (color[to] == "GRAY") {
      parent[to] = v;
      cycle_vertex = v;
      return true;
    }
  }
  tout[v] = timer++;
  color[v] = "BLACK";
  return false;
}

int main() {
  int m, u, v;
  std::cin >> n >> m;
  g.resize(n);
  tin.resize(n);
  tout.resize(n);
  parent.resize(n);
  color.resize(n, "WHITE");
  std::vector<int> ans;
  for (int i = 0; i < m; ++i) {
    std::cin >> u >> v;
    --u; --v;
    g[u].push_back(v);
  }
  for (int v = 0; v < n; ++v) {
    if (color[v] == "WHITE") {
      if (dfs(v)) {
        std::cout << "YES\n";
        int vertex = parent[cycle_vertex];
        while (vertex != cycle_vertex) {
          ans.push_back(vertex + 1);
          vertex = parent[vertex];
        }
        ans.push_back(cycle_vertex + 1);
        std::reverse(ans.begin(), ans.end());
        for (auto i : ans){
          std::cout << i << ' ';
        }
        return 0;
      }
    }
  }
  std::cout << "NO";
  return 0;
}
