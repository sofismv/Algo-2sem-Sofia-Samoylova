#include <iostream>
#include <vector>

bool bit(int mask, int pos) {
  return (mask >> pos) & 1;
}

bool correct(int mask, int mask1, int n) {
  for (int i = 0; i < n - 1; i++) {
    if (bit(mask, i) == bit(mask1, i) && bit(mask, i + 1) == bit(mask1, i + 1) && bit(mask, i) == bit(mask1, i + 1)) {
      return false;
    }
  }
  return true;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  if (m < n) {
    std::swap(m, n);
  }
  int number_of_masks = 1 << n;
  std::vector<std::vector<int>> dp(m, std::vector<int>(number_of_masks));
  for (int j = 0; j < m; j++) {
    for (int mask = 0; mask < number_of_masks; mask++) {
      if (j == 0) {
        dp[j][mask] = 1;
      } else {
        dp[j][mask] = 0;
      }
    }
  }
  for (int j = 0; j < m - 1; j++) {
    for (int mask = 0; mask < number_of_masks; mask++) {
      for (int mask1 = 0; mask1 < number_of_masks; mask1++) {
        if (correct(mask, mask1, n)) {
          dp[j + 1][mask1] += dp[j][mask];
        }
      }
    }
  }
  long long ans = 0;
  for (int mask = 0; mask < number_of_masks; mask++) {
    ans += dp[m - 1][mask];
  }
  std::cout << ans;
  return 0;
}