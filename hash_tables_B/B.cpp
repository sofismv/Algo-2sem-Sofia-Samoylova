#include <vector>
#include <cmath>
#include <iostream>

struct Element {
  int key;
  int value;
  Element(int key, int value) : key(key), value(value) {};
};

class HashTable {
 public:
  explicit HashTable(uint64_t size) : size_(size) {
    for (int i = 0; i < size; ++i) {
      hash_table_.emplace_back(0, 0);
    }
  };
  int Swap(int a, int b);
 private:
  uint64_t HashFunction(int key);
  int Insert(int key);

 private:
  uint64_t size_;
  std::vector<Element> hash_table_;
};

int HashTable::Swap(int a, int b) {
  int a_pos = Insert(a);
  int b_pos = Insert(b);
  int k = abs(hash_table_[a_pos].value - hash_table_[b_pos].value);
  std::swap(hash_table_[a_pos].value, hash_table_[b_pos].value);
  return k;
}
uint64_t HashTable::HashFunction(int key) {
  return ((key % size_) * 31) % size_;
}
int HashTable::Insert(int key) {
  int q = 1;
  int pos = HashFunction(key);
  while (pos < size_ && hash_table_[pos].key != 0 && hash_table_[pos].key != key) {
    pos += q;
    pos %= size_;
  }
  if (hash_table_[pos].key == key) {
    return pos;
  }
  hash_table_[pos] = Element(key, key);
  return pos;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, x, y;
  std::cin >> n;
  HashTable table(500000);
  for (int i = 0; i < n; i++) {
    std::cin >> x >> y;
    std::cout << table.Swap(x, y) << '\n';
  }
  return 0;
}
