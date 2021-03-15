#include <vector>
#include <iostream>

bool check_current(long long x, long long y, long long W, long long H) {
  return (x >= 0 && y >= 0 && x < W && y < H);
}
bool check_left(long long x, long long y, long long W, long long H) {
  return (x - 1 >= 0 && y >= 0 && x - 1 < W && y < H);
}
bool check_down(long long x, long long y, long long W, long long H) {
  return (x >= 0 && y - 1 >= 0 && x < W && y - 1 < H);
}
bool check_right(long long x, long long y, long long W, long long H) {
  return (x + 1 >= 0 && y >= 0 && x + 1 < W && y < H);
}
bool check_up(long long x, long long y, long long W, long long H) {
  return (x >= 0 && y + 1 >= 0 && x < W && y + 1 < H);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  long long W, H, N;
  long long x, y;
  std::cin >> W >> H >> N;
  if (W * H >= 5 * N) {
    for (int i = 0; i < N; ++i) {
      std::cin >> x >> y;
    }
    std::cout << "No";
    return 0;
  }
  std::vector<std::vector<bool>> table(W, std::vector<bool>(H));
  for (int i = 0; i < W; ++i) {
    for (int j = 0; j < H; ++j) {
      table[i][j] = false;
    }
  }
  for (int i = 0; i < N; ++i) {
    std::cin >> x >> y;
    x--;
    y--;
    table[x][y] = check_current(x, y, W, H);
    table[x - 1][y] = check_left(x, y, W, H);
    table[x][y - 1] = check_down(x, y, W, H);
    table[x + 1][y] = check_left(x, y, W, H);
    table[x][y + 1] = check_up(x, y, W, H);
  }
  for (int i = 0; i < W; ++i) {
    for (int j = 0; j < H; ++j) {
      if (!table[i][j]) {
        std::cout << "No";
        return 0;
      }
    }
  }
  std::cout << "Yes";
  return 0;
}
