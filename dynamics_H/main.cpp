#include <vector>
#include <iostream>

int get_answer(int n, int m, const std::vector<std::vector<int>> &dp) {
  int ans = 0;
  for (int i = 0; i < m; i++) {
    ans = std::max(ans, dp[n - 1][i]);
  }
  return ans;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> a(n);
  std::vector<int> b(m);
  std::vector<std::vector<int>> dp(n, std::vector<int>(m));
  for (int i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  for (int i = 0; i < m; i++) {
    std::cin >> b[i];
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      dp[i][j] = -1;
      dp[i][j] = (i == 0 && a[i] == b[j]) ? 1 : 0;
    }
  }
  for (int i = 1; i < n; i++) {
    int max_subsequence = 0;
    for (int j = 0; j < m; j++) {
      dp[i][j] = (a[i] == b[j] && dp[i - 1][j] < max_subsequence + 1) ? max_subsequence + 1 : dp[i][j];
      dp[i][j] = std::max(dp[i - 1][j], dp[i][j]);
      max_subsequence = (a[i] > b[j] && dp[i - 1][j] > max_subsequence) ? dp[i - 1][j] : max_subsequence;
    }
  }
  std::cout << get_answer(n, m, dp);
  return 0;
}