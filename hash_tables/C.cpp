#include <iostream>
#include <vector>

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
    if (x >= 0 && y >= 0 && x < W && y < H) {
      table[x][y] = true;
    }
    if (x - 1 >= 0 && y >= 0 && x - 1 < W && y < H) {
      table[x - 1][y] = true;
    }
    if (x >= 0 && y - 1 >= 0 && x < W && y - 1 < H) {
      table[x][y - 1] = true;
    }
    if (x + 1 >= 0 && y >= 0 && x + 1 < W && y < H) {
      table[x + 1][y] = true;
    }
    if (x >= 0 && y + 1 >= 0 && x < W && y + 1 < H) {
      table[x][y + 1] = true;
    }
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
