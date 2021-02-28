#include <iostream>
#include <vector>

struct Query {
  std::string key;
  std::string value;
  Query(const std::string &str, const std::string &value) : key(str), value(value) {};
};

class HashTable {
  uint64_t size;
  std::vector<std::vector<Query>> hash_table;
 public:
  HashTable(uint64_t size) : size(size) {
    hash_table.resize(size);
  };
  uint64_t HashFunction(const std::string &s) {
    uint64_t hash = 0;
    for (char i : s) {
      hash = 31 * hash + i;
    }
    hash %= size;
    return hash;
  }
  void Put(const std::string &s, const std::string &number) {
    Delete(s);
    uint64_t hash = HashFunction(s);
    hash_table[hash].emplace_back(Query(s, number));
  }
  bool Get(const std::string &s) {
    uint64_t hash = HashFunction(s);
    for (const auto &i : hash_table[hash]) {
      if (i.key == s) {
        std::cout << i.value << '\n';
        return true;
      }
    }
    std::cout << "none" << '\n';
    return false;
  }
  bool Find(const std::string &s) {
    uint64_t hash = HashFunction(s);
    for (const auto &i : hash_table[hash]) {
      if (i.key == s) {
        return true;
      }
    }
    return false;
  }
  void Delete(const std::string &s) {
    uint64_t hash = HashFunction(s);
    if (!Find(s)) {
      return;
    }
    for (int i = 0; i < hash_table[hash].size(); ++i) {
      if (hash_table[hash][i].key == s) {
        hash_table[hash].erase(hash_table[hash].begin() + i);
      }
    }
  }
};
int main() {
  freopen("map.in", "r", stdin);
  freopen("map.out", "w", stdout);
  std::string key, x, y;
  HashTable table(1000);
  while (std::cin >> key) {
    switch (key[0]) {
      case 'p':std::cin >> x >> y;
        table.Put(x, y);
        break;
      case 'd':std::cin >> x;
        table.Delete(x);
        break;
      case 'g':std::cin >> x;
        table.Get(x);
    }
  }
  return 0;
}