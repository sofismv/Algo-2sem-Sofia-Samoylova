#include <list>
#include <vector>
#include <iostream>

struct Query {
  std::string str;
  int value;
  Query(const std::string& str, int value): str(str), value(value) {};
};

class HashTable {
  uint64_t size;
  std::vector<std::vector<Query>> hash_table;
 public:
  HashTable(uint64_t size) :size(size) {
    hash_table.resize(size);
  };
  uint64_t HashFunction (const std::string &s) {
    uint64_t hash = 0;
    for (char i : s) {
      hash = 31 * hash + i;
    }
    hash %= size;
    return hash;
  }
  void Insert(const std::string &s, int number) {
    uint64_t hash = HashFunction(s);
    hash_table[hash].emplace_back(Query(s, number));
  }
  bool Find(const std::string &s) {
    uint64_t hash = HashFunction(s);
    for (const auto& i : hash_table[hash]) {
      if (i.str == s) {
        return true;
      }
    }
    return false;
  }
  int Increment(const std::string &s, int number) {
    uint64_t hash = HashFunction(s);
    for (int i = 0; i < hash_table[hash].size(); ++i) {
      if (hash_table[hash][i].str == s) {
        hash_table[hash][i].value += number;
        return hash_table[hash][i].value;
      }
    }
    return 0;
  }
  void Queries (const std::string &s, int number) {
    if (Find(s)) {
      std::cout << Increment(s, number) << '\n';
    } else {
      Insert(s, number);
      std::cout << number << '\n';
    }
  }

};
int main() {
  std::string s;
  int n;
  HashTable table(1000);
  while (std::cin >> s) {
    std::cin >> n;
    table.Queries(s, n);
  }
    return 0;
}


