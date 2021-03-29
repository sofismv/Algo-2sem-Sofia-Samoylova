#include <vector>
#include <iostream>

int main() {
  int s, n, w;
  std::vector<int> weights;
  std::cin >> s >> n;
  for (int i = 0; i < n; ++i) {
    std::cin >> w;
    weights.push_back(w);
  }
  int dp[n + 1][s + 1]; // max вес выбрать среди первых i элементов, что он <= j
  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= s; ++j) {
      dp[i][j] = -1e9;
    }
  }
  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= s; ++j) {
      if (i == 0 || j == 0) {
        dp[i][j] = 0;
      } else {
        dp[i][j] = dp[i - 1][j];
        if (j - weights[i - 1] >= 0 && dp[i - 1][j - weights[i - 1]] + weights[i - 1] <= s) {
          dp[i][j] = std::max(dp[i - 1][j], dp[i - 1][j - weights[i - 1]] + weights[i - 1]);
        }
      }
    }
  }
  std::cout << dp[n][s];
  return 0;
}