#include <iostream>
#include <vector>

std::vector<std::vector<int>> g;
std::vector<int> tin;
std::vector<int> tout;
int n;
int timer = 0;
std::vector<int> used;

void dfs(int v) {
  tin[v] = timer++;
  used[v] = 1;
  for (int to : g[v]) {
    if (used[to]) {
      continue;
    }
    dfs(to);
  }
  tout[v] = timer++;
}

bool is_parent (int a, int b) {
  return tout[a] > tout[b] && tin[a] < tin[b];
}


int main() {
  int n, parent, m, a, b, root;
  std::cin >> n;
  g.resize(n);
  tin.resize(n);
  tout.resize(n);
  used.resize(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> parent;
    if (parent == 0) {
      root = i;
      continue;
    }
    parent--;
    g[parent].push_back(i);
  }
  dfs(root);
  std::cin >> m;
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b;
    --a; --b;
    std::cout << is_parent(a, b) << '\n';
  }
  return 0;
}
