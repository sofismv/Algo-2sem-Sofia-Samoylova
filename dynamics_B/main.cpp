#include <iostream>
#include <vector>
#include <algorithm>

static int max_int = 2147483646;

void print(const std::vector<int> &v) {
  for (auto i : v) {
    std::cout << i << ' ';
  }
}

void print_answer(int n, const std::vector<int> &dp, const std::vector<int> &parents, const std::vector<int> &p) {
  int k = n;
  while (dp[k] == max_int) {
    --k;
  }
  std::cout << k << std::endl;
  std::vector<int> ans;
  int index = p[k];
  for (int i = 0; i < k; ++i) {
    ans.push_back(n - index);
    index = parents[index];
  }
  print(ans);
}

int main() {
  int n, number;
  std::cin >> n;
  std::vector<int> a;
  std::vector<int> parents(n + 1);
  std::vector<int> p(n + 1);
  for (int i = 0; i < n; ++i) {
    std::cin >> number;
    a.push_back(number);
    parents[i] = -1;
    p[i] = -1;
  }
  parents[n] = -1;
  p[n] = -1;
  std::reverse(a.begin(), a.end());
  std::vector<int> dp(n + 1); // dp[i] - наименьшее число, на которое оканчивается последовательности длины i
  dp.assign(n + 1, max_int);
  dp[0] = -max_int;
  for (int i = 0; i < n; ++i) {
    int j = upper_bound(dp.begin(), dp.end(), a[i]) - dp.begin();
    if (dp[j - 1] <= a[i] && a[i] <= dp[j]) {
      dp[j] = a[i];
      parents[i] = p[j - 1];
      p[j] = i;
    }
  }
  print_answer(n, dp, parents, p);
  return 0;
}
