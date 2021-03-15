#include <iostream>
#include <vector>

int main() {
  int n, m, number;
  std::vector<int> first;
  std::vector<int> second;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::cin >> number;
    first.push_back(number);
  }
  std::cin >> m;
  for (int i = 0; i < m; ++i) {
    std::cin >> number;
    second.push_back(number);
  }
  int dp[n + 1][m + 1];
  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= m; ++j) {
      if (i == 0 || j == 0) {
        dp[i][j] = 0;
      } else {
        if (first[i - 1] != second[j - 1]) {
          dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        } else {
          dp[i][j] = std::max(1 + dp[i - 1][j - 1], std::max(dp[i - 1][j], dp[i][j - 1]));
        }
      }
    }
  }
  std::cout << dp[n][m];
  return 0;
}