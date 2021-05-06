#include <iostream>
#include <vector>
#include <limits>

struct edge {
  int begin;
  int end;
  long long weight;
  edge(int begin_, int end_, long long weight_) : begin(begin_), end(end_), weight(weight_) {}
};

void dfs(std::vector<int> &tin,
         std::vector<int> &tout,
         int &timer,
         std::vector<int> &used,
         const std::vector<std::vector<std::pair<int, int>>> &g,
         int v) {
  tin[v] = timer++;
  used[v] = 1;
  for (auto to : g[v]) {
    if (used[to.first]) {
      continue;
    }
    dfs(tin, tout, timer, used, g, to.first);
  }
  tout[v] = timer++;
}

void Bellman_Ford(std::vector<int> &tin,
                  std::vector<int> &tout,
                  int &timer,
                  std::vector<int> &used,
                  const std::vector<std::vector<std::pair<int, int>>> &g,
                  const std::vector<edge> &edges,
                  std::vector<long long> &dp,
                  int start,
                  long long inf,
                  int n,
                  int m) {
  dp[start] = 0;
  for (int i = 0; i < n - 1; ++i) {
    for (int j = 0; j < m; ++j) {
      if (dp[edges[j].begin] != inf && dp[edges[j].end] > dp[edges[j].begin] + edges[j].weight) {
        dp[edges[j].end] = dp[edges[j].begin] + edges[j].weight;
      }
    }
  }
  for (int i = 0; i < m; ++i) {
    if (dp[edges[i].begin] != inf && dp[edges[i].end] > dp[edges[i].begin] + edges[i].weight) {
      dfs(tin, tout, timer, used, g, edges[i].begin);
    }
  }
}

void print_answer(const std::vector<long long> &dp,
                  const std::vector<int> &used,
                  int n,
                  long long inf) {
  for (int i = 0; i < n; ++i) {
    if (used[i]) {
      std::cout << "-\n";
    } else {
      if (dp[i] == inf) {
        std::cout << "*\n";
      } else {
        std::cout << dp[i] << '\n';
      }
    }
  }
}

int main() {
  int n, m, start, begin, end, timer = 0;
  long long weight;
  long long inf = std::numeric_limits<long long>::max();
  std::cin >> n >> m >> start;
  --start;
  std::vector<edge> edges;
  std::vector<std::vector<std::pair<int, int>>> g(n);
  std::vector<int> tin(n);
  std::vector<int> tout(n);
  std::vector<int> used(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> begin >> end >> weight;
    --begin;
    --end;
    g[begin].push_back(std::make_pair(end, weight));
    edges.emplace_back(begin, end, weight);
  }
  std::vector<long long> dp(n, inf);
  Bellman_Ford(tin, tout, timer, used, g, edges, dp, start, inf, n, m);
  print_answer(dp, used, n, inf);
  return 0;
}
