#include <iostream>
#include <vector>
#include <set>
#include <limits>

int Dijkstra(int n,
              int start,
              int finish,
              const std::vector<std::vector<std::pair<int, int>>> &g,
              int inf) {
  std::vector<int> dist(n, inf);
  std::set<std::pair<int, int>> dp;
  std::vector<bool> used(n, false);
  dist[start] = 0;
  dp.insert(std::make_pair(0, start));
  while (!dp.empty()) {
    std::pair<int, int> edge_ = *(dp.begin());
    dp.erase(dp.begin());
    for (auto next_edge : g[edge_.second]) {
      if (dist[next_edge.second] > dist[edge_.second] + next_edge.first) {
        if (dist[next_edge.second] != inf) {
          auto it = dp.find(std::make_pair(dist[next_edge.second], next_edge.second));
          dp.erase(it);
        }
        dist[next_edge.second] = dist[edge_.second] + next_edge.first;
        dp.insert(std::make_pair(dist[next_edge.second], next_edge.second));
      }
    }
  }
  if (dist[finish] == inf) {
    dist[finish] = -1;
  }
  return dist[finish];
}

int main() {
  int n, m, begin, end, weight, start, finish;
  int inf = std::numeric_limits<int>::max();
  std::cin >> n >> m;
  std::cin >> start >> finish;
  --start;
  --finish;
  std::vector<std::vector<std::pair<int, int>>> g(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> begin >> end >> weight;
    g[--begin].push_back(std::make_pair(weight, --end));
    g[end].push_back(std::make_pair(weight, begin));
  }
  std::cout << Dijkstra(n, start, finish, g, inf);
  return 0;
}