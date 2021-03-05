#include <vector>
#include <iostream>

struct Query {
  std::string str;
  int value;
  Query(const std::string &str, int value) : str(str), value(value) {};
};

class HashTable {
 public:
  int Incrementator(const std::string &s, int number);
  explicit HashTable(uint64_t size) : size_(size) {
    hash_table_.resize(size);
  }
 private:
  uint64_t HashFunction(const std::string &s);
  void Insert(const std::string &s, int number);
  bool Find(const std::string &s);
  int Increment(const std::string &s, int number);

 private:
  uint64_t size_;
  std::vector<std::vector<Query>> hash_table_;
};

int HashTable::Incrementator(const std::string &s, int number) {
  if (Find(s)) {
    return Increment(s, number);
  } else {
    Insert(s, number);
    return number;
  }
}
uint64_t HashTable::HashFunction(const std::string &s) {
  uint64_t hash = 0;
  for (char i : s) {
    hash = 31 * hash + i;
  }
  hash %= size_;
  return hash;
}
void HashTable::Insert(const std::string &s, int number) {
  uint64_t hash = HashFunction(s);
  hash_table_[hash].emplace_back(Query(s, number));
}
bool HashTable::Find(const std::string &s) {
  uint64_t hash = HashFunction(s);
  for (const auto &i : hash_table_[hash]) {
    if (i.str == s) {
      return true;
    }
  }
  return false;
}
int HashTable::Increment(const std::string &s, int number) {
  uint64_t hash = HashFunction(s);
  for (int i = 0; i < hash_table_[hash].size(); ++i) {
    if (hash_table_[hash][i].str == s) {
      hash_table_[hash][i].value += number;
      return hash_table_[hash][i].value;
    }
  }
  return 0;
}

int main() {
  std::string s;
  int n;
  HashTable table(1000);
  while (std::cin >> s) {
    std::cin >> n;
    std::cout << table.Incrementator(s, n) << '\n';
  }
  return 0;
}
