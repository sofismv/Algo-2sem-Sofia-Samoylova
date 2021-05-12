#include <iostream>
#include <cmath>
#include <vector>

template<typename T, typename Allocator = std::allocator<T>>
class List {
 public:
  struct Node {
    T value;
    Node *prev = nullptr;
    Node *next = nullptr;

    Node(const T &value_) : value(value_) {}
    Node(T &&value_) : value(std::move(value_)) {}
    template<typename ...Args>
    Node(Args &&... args): value(std::forward<Args>(args)...) {}
  };
 private:
  using node_alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
  Node *fake;
  size_t size_ = 0;
  node_alloc allocator;

 public:

  explicit List(const Allocator &alloc = Allocator()) : allocator(alloc) {
    fake = allocator.allocate(1);
    fake->prev = fake;
    fake->next = fake;
  }

  List(size_t count, const T &value, const Allocator &alloc = Allocator()) : List(alloc) {
    for (size_t i = 0; i < count; ++i) {
      push_back(value);
    }
  }

  List(size_t count, const Allocator &alloc = Allocator()) : List(alloc) {
    for (size_t i = 0; i < count; ++i) {
      Node *new_node = std::allocator_traits<node_alloc>::allocate(allocator, 1);
      std::allocator_traits<node_alloc>::construct(allocator, new_node);
      Node *new_node_prev = fake->prev;
      Node *new_node_next = fake;
      new_node->next = new_node_next;
      new_node->prev = new_node_prev;
      fake->prev->next = new_node;
      fake->prev = new_node;
    }
    size_ = count;
  }

  List(const List &other)
      : List(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator())) {
    for (auto it = other.cbegin(); it != other.cend(); ++it) {
      push_back(*it);
    }
  }

  List(List &&other) noexcept:
      fake(other.fake),
      size_(other.size_),
      allocator(std::move(other.allocator)) {
    other.fake = nullptr;
    other.size_ = 0;
  }

  List &operator=(const List &other) {
    bool alloc_copy = std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value;
    if (this == &other) {
      return *this;
    }
    while (size_ != 0) {
      pop_back();
    }
    allocator.deallocate(fake, 1);
    if (alloc_copy) {
      allocator = other.get_allocator();
    }
    fake = allocator.allocate(1);
    fake->next = fake;
    fake->prev = fake;
    for (auto it = other.cbegin(); it != other.cend(); ++it) {
      push_back(*it);
    }
    return *this;
  }

  List &operator=(List &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    while (size_ != 0) {
      pop_back();
    }
    List temp = std::move(other);
    std::swap(size_, temp.size_);
    std::swap(fake, temp.fake);
    std::swap(allocator, temp.allocator);
    return *this;
  }

  size_t size() const {
    return size_;
  }

  template<bool is_const>
  struct common_iterator {
   private:
    friend class List;
    friend class iterator;
    friend class const_iterator;

    Node *ptr;

   public:
    using difference_type = std::ptrdiff_t;
    using value_type = typename std::conditional<is_const, const T, T>::type;
    using pointer = typename std::conditional<is_const, const T *, T *>::type;
    using reference = typename std::conditional<is_const, const T &, T &>::type;
    using iterator_category = std::bidirectional_iterator_tag;

    common_iterator() = default;
    common_iterator(Node *ptr_) : ptr(ptr_) {}

    common_iterator &operator++() {
      ptr = ptr->next;
      return *this;
    }
    common_iterator &operator--() {
      ptr = ptr->prev;
      return *this;
    }
    common_iterator operator++(int) {
      common_iterator copy = *this;
      ptr = ptr->next;
      return copy;
    }
    common_iterator operator--(int) {
      common_iterator copy = *this;
      ptr = ptr->prev;
      return copy;
    }
    bool operator==(const common_iterator &other) const {
      return ptr == other.ptr;
    }
    bool operator!=(const common_iterator &other) const {
      return ptr != other.ptr;
    }
    reference operator*() {
      return ptr->value;
    }
    pointer operator->() {
      return &ptr->value;
    }
    operator common_iterator<true>() {
      return common_iterator<true>(ptr);
    }
    common_iterator(const common_iterator<true> &other) : ptr(other.ptr) {}
  };
  using const_iterator = common_iterator<true>;
  using iterator = common_iterator<false>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  void push_back(const T &value) {
    Node *new_node = std::allocator_traits<node_alloc>::allocate(allocator, 1);
    std::allocator_traits<node_alloc>::construct(allocator, new_node, value);
    Node *new_node_prev = fake->prev;
    Node *new_node_next = fake;
    new_node->next = new_node_next;
    new_node->prev = new_node_prev;
    fake->prev->next = new_node;
    fake->prev = new_node;
    size_++;
  }
  template<typename T_>
  void push_back(T_ &&value) {
    Node *new_node = std::allocator_traits<node_alloc>::allocate(allocator, 1);
    std::allocator_traits<node_alloc>::construct(allocator, new_node, std::forward<T_>(value));

    Node *new_node_prev = fake->prev;
    Node *new_node_next = fake;
    new_node->next = new_node_next;
    new_node->prev = new_node_prev;
    fake->prev->next = new_node;
    fake->prev = new_node;
    size_++;
  }

  template<typename T_>
  iterator push_front(T_ &&value) {
    Node *new_node = std::allocator_traits<node_alloc>::allocate(allocator, 1);
    std::allocator_traits<node_alloc>::construct(allocator, new_node, std::forward<T_>(value));

    Node *new_node_prev = fake;
    Node *new_node_next = fake->next;
    new_node->next = new_node_next;
    new_node->prev = new_node_prev;
    fake->next->prev = new_node;
    fake->next = new_node;
    size_++;
    return iterator(new_node);
  }
  void pop_back() {
    erase(--end());
  }
  void pop_front() {
    erase(begin());
  }

  Allocator get_allocator() const {
    return allocator;
  }

  iterator begin() {
    return iterator(fake->next);
  }

  const_iterator begin() const {
    return const_iterator(fake->next);
  }

  iterator end() {
    return iterator(fake);
  }

  const_iterator end() const {
    return const_iterator(fake);
  }

  const_iterator cbegin() const {
    return const_iterator(fake->next);
  }

  const_iterator cend() const {
    return const_iterator(fake);
  }

  const_reverse_iterator rbegin() const {
    return std::make_reverse_iterator(end());
  }

  const_reverse_iterator rend() const {
    return std::make_reverse_iterator(begin());
  }

  reverse_iterator rbegin() {
    return std::make_reverse_iterator(end());
  }

  reverse_iterator rend() {
    return std::make_reverse_iterator(begin());
  }

  const_reverse_iterator crbegin() const {
    return std::make_reverse_iterator(cend());
  }

  const_reverse_iterator crend() const {
    return std::make_reverse_iterator(cbegin());
  }

  void erase(const_iterator it) {
    Node *next = it.ptr->next;
    Node *prev = it.ptr->prev;
    prev->next = next;
    next->prev = prev;
    std::allocator_traits<node_alloc>::destroy(allocator, it.ptr);
    allocator.deallocate(it.ptr, 1);
    --size_;
  }

  template<typename T_>
  iterator insert(const_iterator it, T_ &&value) {
    Node *new_node = std::allocator_traits<node_alloc>::allocate(allocator, 1);
    std::allocator_traits<node_alloc>::construct(allocator, new_node, std::forward<T_>(value));
    Node *new_node_next = it.ptr;
    Node *new_node_prev = it.ptr->prev;
    new_node_next->prev = new_node;
    new_node->prev = new_node_prev;
    new_node->next = new_node_next;
    new_node_prev->next = new_node;
    ++size_;
    return iterator(new_node);
  }

  iterator insert(const_iterator it, T &&value) {
    Node *new_node = std::allocator_traits<node_alloc>::allocate(allocator, 1);
    std::allocator_traits<node_alloc>::construct(allocator, new_node, std::move(value));
    Node *new_node_next = it.ptr;
    Node *new_node_prev = it.ptr->prev;
    new_node_next->prev = new_node;
    new_node->prev = new_node_prev;
    new_node->next = new_node_next;
    new_node_prev->next = new_node;
    ++size_;
    return iterator(new_node);
  }

  template<typename... Args>
  void emplace(Args &&... args) {
    Node *new_node = std::allocator_traits<node_alloc>::allocate(allocator, 1);
    std::allocator_traits<node_alloc>::construct(allocator, &new_node->value, std::forward<Args>(args)...);
    new_node->next = fake;
    new_node->prev = fake->prev;
    (fake->prev)->next = new_node;
    fake->prev = new_node;
    ++size_;
  }

  ~List() {
    if (fake == nullptr) {
      return;
    }
    while (size_) {
      pop_back();
    }
    allocator.deallocate(fake, 1);
  }
};

template<
    typename Key,
    typename Value,
    typename Hash = std::hash<Key>,
    typename Equal = std::equal_to<Key>,
    typename Alloc = std::allocator<std::pair<const Key, Value>>>
class UnorderedMap {
 public:
  using NodeType = std::pair<const Key, Value>;
  using Iterator = typename List<NodeType, Alloc>::iterator;
  using ConstIterator = typename List<NodeType, Alloc>::const_iterator;
  using BucketIterator = typename List<NodeType, Alloc>::iterator;
 private:
  struct Bucket {
    typename List<Iterator, Alloc>::Iterator bucket_iter;
    size_t bucket_size;
    Bucket() : bucket_iter(nullptr), bucket_size(0) {}
    Bucket(Iterator bucket_iter_, size_t bucket_size_) : bucket_iter(bucket_iter_), bucket_size(bucket_size_) {}
  };

  List<NodeType, Alloc> elements;
  List<Iterator, Alloc> pointers;
  std::vector<std::vector<BucketIterator>> hash_table;

  Hash hash_func_;
  Equal key_eq;
  Alloc alloc_;
  float max_load_factor_ = 0.75;

  void resize_if_necessary() {
    if (load_factor() > max_load_factor()) {
      reserve(2 * bucket_count());
    }
  }

  size_t hash_function(const Key &key) {
    return hash_func_(key) % bucket_count();
  }

  void move_everything(UnorderedMap &&other) {
    hash_table = std::move(other.hash_table);
    hash_func_ = std::move(other.hash_func_);
    key_eq = std::move(other.key_eq);
    max_load_factor_ = other.max_load_factor_;
    elements = std::move(other.elements);
    alloc_ = std::move(other.alloc_);
  }

  Iterator insert_list_node(NodeType *list_node) {
    size_t hash = hash_function(list_node->first);
    elements.emplace(*list_node);
    auto it = --elements.end();
    hash_table[hash].push_back(it);
    alloc_.deallocate(list_node, 1);
    resize_if_necessary();
    return it;
  }

  std::pair<Iterator, bool> insert_(NodeType *node) {
    if (node == nullptr) {
      return {end(), false};
    }
    auto it = find(node->first);
    if (it != end()) {
      return {it, false};
    }
    return std::make_pair(insert_list_node(node), true);
  }

 public:
  /*void print() {
    for (auto i: elements) {
      std::cout << i.first << ' ' << i.second << std::endl;
    }
  }*/

  UnorderedMap() {
    hash_table.resize(1);
  }

  UnorderedMap(const UnorderedMap &other)
      : hash_func_(other.hash_func_),
        key_eq(other.key_eq),
        alloc_(other.alloc_),
        max_load_factor_(other.max_load_factor_) {
    hash_table.resize(other.bucket_count());
    for (auto &it : other) {
      insert(it);
    }
  }

  UnorderedMap(UnorderedMap &&other) noexcept {
    move_everything(std::move(other));
  }

  UnorderedMap &operator=(UnorderedMap &&other) noexcept {
    if (this != &other) {
      UnorderedMap temp(std::move(other));
      move_everything(std::move(temp));
    }
    return *this;
  }

  UnorderedMap &operator=(const UnorderedMap &other) {
    if (this != &other) {
      UnorderedMap temp(other);
      move_everything(std::move(temp));
    }
    return *this;
  }

  ~UnorderedMap() {
    hash_table.clear();
  }

  size_t size() const {
    return elements.size();
  }

  bool empty() const noexcept {
    return size() == 0;
  }

  size_t bucket_count() const {
    return hash_table.size();
  }

  float load_factor() const {
    return static_cast<float>(size()) / bucket_count();
  }

  float max_load_factor() const {
    return max_load_factor_;
  }

  void max_load_factor(float ml) {
    max_load_factor_ = ml;
    resize_if_necessary();
  }

  void reserve(size_t count) {
    rehash(std::ceil(count / max_load_factor()));
  }

  void rehash(size_t count) {
    hash_table.clear();
    hash_table.resize(count);
    auto iter = elements.begin();
    while (iter != elements.end()) {
      size_t hash = hash_function(iter->first);
      hash_table[hash].push_back(iter);
      ++iter;
    }
  }

  size_t max_size() const noexcept {
    return max_load_factor() * bucket_count();
  }

  Iterator find(const Key &key) {
    size_t hash = hash_function(key);
    if (hash_table[hash].size() == 0) {
      return end();
    }
    std::vector<BucketIterator> current_bucket = hash_table[hash];
    for (size_t i = 0; i < current_bucket.size(); ++i) {
      if (key_eq(current_bucket[i]->first, key)) {
        return current_bucket[i];
      }
    }
    return end();
  }

  Value &operator[](const Key &key) {
    Iterator it = find(key);
    if (it == end()) {
      return emplace(key, Value()).first->second;
    }
    return it->second;
  }

  Value &at(const Key &key) {
    Iterator it = find(key);
    if (it == end()) {
      throw std::out_of_range("Such element does not exist.");
    }
    return it->second;
  }

  const Value &at(const Key &key) const {
    Iterator it = find(key);
    if (it == end()) {
      throw std::out_of_range("Such element does not exist.");
    }
    return it->second;
  }

  std::pair<Iterator, bool> insert(const NodeType &value) {
    NodeType *node = std::allocator_traits<Alloc>::allocate(alloc_, 1);
    std::allocator_traits<Alloc>::construct(alloc_, node, value);
    return insert_(node);
  }

  template<typename NodePair>
  std::pair<Iterator, bool> insert(NodePair &&value) {
    return emplace(std::forward<NodePair>(value));
  }

  template<typename... Args>
  std::pair<Iterator, bool> emplace(Args &&... args) {
    elements.emplace(std::forward<Args>(args)...);
    auto it = elements.end();
    --it;
    if (find(it->first) != end()) {
      elements.pop_back();
      return {find(it->first), false};
    }
    size_t hash = hash_function(it->first);
    hash_table[hash].push_back(it);
    resize_if_necessary();
    return {--end(), true};
  }

  template<typename InputIt>
  void insert(const InputIt &first, const InputIt &last) {
    for (auto it = first; it != last; ++it) {
      insert(*it);
    }
  }

  Iterator erase(ConstIterator pos) {
    auto next_iter = find(pos->first);
    ++next_iter;
    size_t hash = hash_function(pos->first);
    size_t i = 0;
    while (!key_eq(pos->first, hash_table[hash][i]->first)) {
      ++i;
    }
    std::swap(hash_table[hash][i], hash_table[hash].back());
    elements.erase(pos);
    hash_table[hash].pop_back();
    return next_iter;
  }

  Iterator erase(ConstIterator first, ConstIterator last) {
    for (auto it = first; it != last; ++it) {
      erase(it);
    }
    return last;
  }

  Iterator begin() {
    return elements.begin();
  }

  ConstIterator begin() const {
    return elements.cbegin();
  }

  Iterator end() {
    return elements.end();
  }

  ConstIterator end() const {
    return elements.cend();
  }

  ConstIterator cbegin() const {
    return elements.cbegin();
  }

  ConstIterator cend() const {
    return elements.cend();
  }
};

