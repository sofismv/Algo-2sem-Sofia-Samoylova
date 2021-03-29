#include <iostream>
#include <vector>

bool bit(int mask, int pos) {
  return (mask >> pos) & 1;
}

void initialize(std::vector<std::vector<int>> &dp,
                std::vector<std::vector<std::pair<int, int>>> &prev,
                int n,
                int number_of_masks) {
  for (int mask = 1; mask < number_of_masks; mask++) {
    for (int v = 0; v < n; v++) {
      if (mask == 1 << v) {
        dp[v][mask] = 0;
      } else {
        dp[v][mask] = 10e8;
      }
      prev[v][mask] = std::make_pair(0, 0);
    }
  }
}

void counting_dp(std::vector<std::vector<int>> &g,
                 std::vector<std::vector<int>> &dp,
                 std::vector<std::vector<std::pair<int, int>>> &prev,
                 int n,
                 int number_of_masks) {
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

void counting_answer(std::vector<std::vector<int>> &dp,
                     std::vector<std::vector<std::pair<int, int>>> &prev,
                     std::vector<int> &vertices,
                     int n,
                     int number_of_masks,
                     int &ans,
                     int mask) {
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

int main() {
  int n, c;
  std::cin >> n;
  std::vector<std::vector<int>> g(n, std::vector<int>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; ++j) {
      std::cin >> g[i][j];
    }
  }
  int number_of_masks = 1 << n;
  std::vector<std::vector<int>> dp(n, std::vector<int>(number_of_masks));
  std::vector<std::vector<std::pair<int, int>>> prev(n, std::vector<std::pair<int, int>>(number_of_masks));
  initialize(dp, prev, n, number_of_masks);
  counting_dp(g, dp, prev, n, number_of_masks);
  int ans = 10e8, mask = number_of_masks - 1;
  std::vector<int> vertices;
  counting_answer(dp, prev, vertices, n, number_of_masks, ans, mask);
  std::cout << ans << '\n';
  for (int i = 0; i < n; i++) {
    std::cout << vertices[n - i - 1] + 1 << ' ';
  }
  return 0;
}