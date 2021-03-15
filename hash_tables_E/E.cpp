#include <vector>
#include <iostream>
#include <fstream>

struct Query {
  std::string key;
  std::string value;
  Query(const std::string &str, const std::string &value) : key(str), value(value) {};
};

class HashTable {
 public:
  HashTable(uint64_t size) : size_(size) {
    hash_table_.resize(size);
  };
  void Delete(const std::string &s);
  void Put(const std::string &s, const std::string &number);
  std::string Get(const std::string &s);
 private:
  uint64_t HashFunction(const std::string &s);
  bool Find(const std::string &s);

 private:
  uint64_t size_;
  std::vector<std::vector<Query>> hash_table_;
};

uint64_t HashTable::HashFunction(const std::string &s) {
  uint64_t hash = 0;
  for (char i : s) {
    hash = 31 * hash + i;
  }
  hash %= size_;
  return hash;
}
void HashTable::Put(const std::string &s, const std::string &number) {
  Delete(s);
  uint64_t hash = HashFunction(s);
  hash_table_[hash].emplace_back(Query(s, number));
}
std::string HashTable::Get(const std::string &s) {
  uint64_t hash = HashFunction(s);
  for (const auto &i : hash_table_[hash]) {
    if (i.key == s) {
      return i.value;
    }
  }
  return "none";
}
bool HashTable::Find(const std::string &s) {
  uint64_t hash = HashFunction(s);
  for (const auto &i : hash_table_[hash]) {
    if (i.key == s) {
      return true;
    }
  }
  return false;
}
void HashTable::Delete(const std::string &s) {
  uint64_t hash = HashFunction(s);
  if (!Find(s)) {
    return;
  }
  for (int i = 0; i < hash_table_[hash].size(); ++i) {
    if (hash_table_[hash][i].key == s) {
      hash_table_[hash].erase(hash_table_[hash].begin() + i);
    }
  }
}

int main() {
  std::ifstream file_in ("map.in");
  std::string key, x, y;
  HashTable table(1000);
  std::ofstream file_out ("map.out");
  while (file_in >> key) {
    switch (key[0]) {
      case 'p':
        file_in >> x >> y;
        table.Put(x, y);
        break;
      case 'd':
        file_in >> x;
        table.Delete(x);
        break;
      case 'g':
        file_in >> x;
        file_out << table.Get(x) << '\n';
    }
  }
  return 0;
}