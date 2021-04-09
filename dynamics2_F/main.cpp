#include <iostream>
#include <vector>
#include <limits>

bool bit(int mask, int pos) {
  return (mask >> pos) & 1;
}

class dp {
 private:
  std::vector<std::vector<int>> dp;
  std::vector<std::vector<std::pair<int, int>>> prev;
  int n;
  int number_of_masks;

 public:
  void initialize(int n_) {
    n = n_;
    number_of_masks = 1 << n;
    dp.resize(n, std::vector<int>(number_of_masks));
    prev.resize(n, std::vector<std::pair<int, int >>(number_of_masks));
    for (int mask = 1; mask < number_of_masks; mask++) {
      for (int v = 0; v < n; v++) {
        if (mask == 1 << v) {
          dp[v][mask] = 0;
        } else {
          dp[v][mask] = std::numeric_limits<int>::max();
        }
        prev[v][mask] = std::make_pair(0, 0);
      }
    }
  }
  void counting_dp(const std::vector<std::vector<int>> &g) {
    for (int mask = 1; mask < number_of_masks; mask++) {
      for (int v = 0; v < n; v++) {
        for (int u = 0; u < n; u++) {
          if (bit(mask, u)) {
            continue;
          }
          int new_mask = mask | (1 << u);
          if (dp[u][new_mask] > dp[v][mask] + g[v][u]) {
            dp[u][new_mask] = dp[v][mask] + g[v][u];
            prev[u][new_mask] = std::make_pair(v, mask);
          }
        }
      }
    }
  }
  void counting_answer(std::vector<int> &vertices, int &ans) {
    int mask = number_of_masks - 1;
    int vert;
    for (int v = 0; v < n; v++) {
      if (ans > dp[v][number_of_masks - 1]) {
        ans = dp[v][number_of_masks - 1];
        vert = v;
      }
    }
    vertices.push_back(vert);
    for (int i = 1; i < n; ++i) {
      vertices.push_back(prev[vert][mask].first);
      std::pair<int, int> pr = prev[vert][mask];
      vert = pr.first;
      mask = pr.second;
    }
  }
};

int main() {
  int n, c;
  std::cin >> n;
  std::vector<std::vector<int>> g(n, std::vector<int>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; ++j) {
      std::cin >> g[i][j];
    }
  }
  dp dynamics;
  dynamics.initialize(n);
  dynamics.counting_dp(g);
  int ans = std::numeric_limits<int>::max();
  std::vector<int> vertices;
  dynamics.counting_answer(vertices, ans);
  std::cout << ans << '\n';
  for (int i = 0; i < n; i++) {
    std::cout << vertices[n - i - 1] + 1 << ' ';
  }
  return 0;
}
