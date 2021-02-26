#include <iostream>
#include <vector>
#include <cmath>
// first - key, second - position

class HashTable {
  uint64_t size;
  std::vector<std::pair<int, int>> hash_table;
 public:
  HashTable(uint64_t size) : size(size) {
    hash_table.resize(size);
    for (int i = 0; i < size; ++i) {
      hash_table[i] = std::make_pair(0, 0);
    }
  };
  uint64_t HashFunction(int key) {
    return ((key % size) * 31) % size;
  }
  int Insert(int key) {
    int q = 1;
    int pos = HashFunction(key);
    while (pos < size && hash_table[pos].first != 0 && hash_table[pos].first != key) {
      pos += q;
      pos %= size;
    }
    if (hash_table[pos].first == key) {
      return pos;
    }
    hash_table[pos] = std::make_pair(key, key);
    return pos;
  }
  int Find(int key) {
    int pos = 0;
    int q = HashFunction(key);
    while (pos < size && hash_table[pos].first != key) {
      pos += q;
    }
    if (pos >= size) {
      return -1;
    }
    return pos;
  }
  int Swap(int a, int b) {
    int a_pos = Insert(a);
    int b_pos = Insert(b);
    int k = abs(hash_table[a_pos].second - hash_table[b_pos].second);
    std::swap(hash_table[a_pos].second, hash_table[b_pos].second);
    return k;
  }
};
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int n, x, y;
  std::cin >> n;
  HashTable table(500000);
  for (int i = 0; i < n; i++) {
    std::cin >> x >> y;
    std::cout << table.Swap(x, y) << '\n';
  }
  return 0;
}
