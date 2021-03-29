#include <iostream>
#include <vector>

class BigInteger {
 private:
  static const int BASE = 10000;
  static const int BASE_LENGTH = 4;
  std::vector<long long> digits;
  bool sign = false;

 public:
  BigInteger();
  BigInteger(const long long &number);
  BigInteger(const std::string &str_number);
  std::string toString() const;
  BigInteger &operator=(const int &num);

  BigInteger &operator+=(const BigInteger &other);
  BigInteger &operator-=(const BigInteger &other);
  friend BigInteger operator-(const BigInteger &left, const BigInteger &right);

  bool isEven() const;

  friend bool operator==(const BigInteger &left, const BigInteger &right);
  friend bool operator<=(const BigInteger &left, const BigInteger &right);
  friend bool operator>(const BigInteger &left, const BigInteger &right);

  static std::string convert_to_binary(const BigInteger &number);

  BigInteger &split_in_half();
  BigInteger &operator++();
  BigInteger &operator--();
  BigInteger operator-() const;

  friend std::ostream &operator<<(std::ostream &out, const BigInteger &num);
  friend std::istream &operator>>(std::istream &in, BigInteger &num);
};
BigInteger::BigInteger() {
  *this = 0;
}
BigInteger::BigInteger(const long long int &number) {
  long long tmp_number = number;
  digits = std::vector<long long>();
  if (number < 0) {
    sign = true;
    tmp_number *= -1;
  }
  while (tmp_number > 0) {
    digits.push_back(tmp_number % BASE);
    tmp_number /= BASE;
  }
}
BigInteger::BigInteger(const std::string &str_number) {
  std::string number;
  if (str_number[0] == '-' && str_number != "-0") {
    sign = true;
    number = str_number.substr(1);
  } else {
    number = str_number;
  }
  unsigned int number_end = number.length();
  while (number_end > BASE_LENGTH) {
    unsigned int number_begin = number_end - BASE_LENGTH;
    std::string digit = number.substr(number_begin, BASE_LENGTH);
    digits.push_back(stoi(digit));
    number_end -= BASE_LENGTH;
  }
  std::string digit = number.substr(0, number_end);
  digits.push_back(stoi(digit));
}
std::string BigInteger::toString() const {
  if (digits.empty()) {
    return "0";
  }
  std::string result = "";
  if (sign) {
    result += "-";
  }
  for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
    if (it == digits.rbegin()) {
      result += std::to_string(*it);
    } else {
      for (unsigned int i = 0; i < BASE_LENGTH - std::to_string(*it).length(); ++i) {
        result += "0";
      }
      result += std::to_string(*it);
    }
  }
  return result;
}
BigInteger &BigInteger::operator=(const int &num) {
  *this = BigInteger(num);
  return *this;
}
BigInteger &BigInteger::split_in_half() {
  int carry = 0;
  for (int i = digits.size() - 1; i >= 0; --i) {
    long long current = digits[i] + carry * BASE;
    digits[i] = current / 2;
    carry = current % 2;
  }
  while (!digits.empty() && digits.back() == 0) {
    digits.pop_back();
  }
  return *this;
}
BigInteger &BigInteger::operator++() {
  if (*this == 0) {
    *this = 1;
    return *this;
  }
  if (*this == -1) {
    *this = 0;
    return *this;
  }
  if (sign) {
    sign = !sign;
    --*this;
    sign = !sign;
    return *this;
  }
  bool need_addition = true;
  unsigned int cur_digit = 0;
  while (need_addition) {
    if (digits.size() == cur_digit) {
      digits.push_back(0);
    }
    ++digits[cur_digit];
    if (digits[cur_digit] < BASE) {
      need_addition = false;
    } else {
      digits[cur_digit] = 0;
    }
    ++cur_digit;
  }
  return *this;
}
bool operator==(const BigInteger &left, const BigInteger &right) {
  return left <= right && right <= left;
}
bool operator<=(const BigInteger &left, const BigInteger &right) {
  if (left.digits.empty() && (!right.sign || right.digits.empty())) {
    return true;
  }
  unsigned int max_size = left.digits.size() > right.digits.size() ? left.digits.size() : right.digits.size();
  if (left.sign && !right.sign) {
    return true;
  }
  if (!left.sign && right.sign) {
    return false;
  }
  if (left.sign && right.sign) {
    return -right <= -left;
  }
  unsigned int first;
  unsigned int second;
  for (unsigned int i = max_size; i > 0; --i) {
    first = i - 1 < left.digits.size() ? left.digits[i - 1] : 0;
    second = i - 1 < right.digits.size() ? right.digits[i - 1] : 0;
    if (first == second) {
      continue;
    } else {
      return first <= second;
    }
  }
  return true;
}
BigInteger &BigInteger::operator--() {
  if (*this == 0) {
    *this = -1;
    return *this;
  }
  if (*this == 1) {
    *this = 0;
    return *this;
  }
  if (sign) {
    sign = !sign;
    ++*this;
    sign = !sign;
    return *this;
  }
  unsigned int cur_digit = 0;
  bool need_loan = true;
  while (need_loan) {
    if (digits[cur_digit] > 0) {
      --digits[cur_digit];
      need_loan = false;
    } else {
      digits[cur_digit] = BASE - 1;
    }
    ++cur_digit;
  }
  if (digits[digits.size() - 1] == 0) {
    digits.pop_back();
  }
  return *this;
}
std::ostream &operator<<(std::ostream &out, const BigInteger &num) {
  out << num.toString();
  return out;
}
std::istream &operator>>(std::istream &in, BigInteger &num) {
  std::string str;
  in >> str;
  num = BigInteger(str);
  return in;
}
BigInteger BigInteger::operator-() const {
  BigInteger result = *this;
  result.sign = !sign;
  return result;
}
std::string BigInteger::convert_to_binary(const BigInteger &number) {
  BigInteger copy = number;
  std::string res;
  while (copy > 0) {
    if (copy.isEven()) {
      res += '0';
    } else {
      res += '1';
    }
    copy.split_in_half();
  }
  return res;
}
bool operator>(const BigInteger &left, const BigInteger &right) {
  return right <= left && !(right == left);
}
bool BigInteger::isEven() const {
  return (digits[0] % 2 == 0);
}
BigInteger &BigInteger::operator+=(const BigInteger &other) {
  if (other == 0) {
    return *this;
  }
  if (sign && !other.sign) {
    sign = !sign;
    *this -= other;
    if (!(*this == 0)) {
      sign = !sign;
    }
    return *this;
  } else {
    if (!sign && other.sign) {
      *this -= -other;
      return *this;
    }
  }
  long long addition = 0;
  long long max_size = digits.size() > other.digits.size() ? digits.size() : other.digits.size();
  long long first;
  long long second;
  long long cur_digit;

  for (unsigned int i = 0; i < max_size; ++i) {
    first = i < digits.size() ? digits[i] : 0;
    second = i < other.digits.size() ? other.digits[i] : 0;
    cur_digit = (first + second + addition) % BASE;
    if (i < digits.size()) {
      digits[i] = cur_digit;
    } else {
      digits.push_back(cur_digit);
    }
    addition = (first + second + addition) / BASE;
  }
  if (addition) {
    digits.push_back(addition);
  }
  return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &other) {
  if (other == 0) {
    return *this;
  }
  if (sign && !other.sign) {
    sign = !sign;
    *this += other;
    sign = !sign;
    return *this;
  }
  if (!sign && other.sign) {
    *this += -other;
    return *this;
  }
  if (sign && other.sign && *this > other) {
    BigInteger result = -other;
    result -= -*this;
    *this = result;
    return *this;
  }
  if (!sign && !other.sign && other > *this) {
    BigInteger result = other;
    result -= *this;
    *this = result;
    sign = !sign;
    return *this;
  }
  long long loan = 0;
  unsigned int max_size = digits.size();
  unsigned int first;
  unsigned int second;
  unsigned int cur_digit;
  for (unsigned int i = 0; i < max_size; ++i) {
    first = i < digits.size() ? digits[i] : 0;
    second = i < other.digits.size() ? other.digits[i] : 0;
    cur_digit = (2 * BASE + first - second - loan) % BASE;
    if (i < digits.size()) {
      digits[i] = cur_digit;
    } else {
      digits.push_back(cur_digit);
    }
    loan = 2 - (2 * BASE + first - second - loan) / BASE;
  }
  while (!digits.empty() && digits[digits.size() - 1] == 0) {
    digits.pop_back();
  }
  if (digits.empty()) {
    sign = false;
  }
  return *this;
}
BigInteger operator-(const BigInteger &left, const BigInteger &right) {
  BigInteger result = left;
  result -= right;
  return result;
}
std::vector<std::vector<long long>> multiply(const std::vector<std::vector<long long>> &first,
                                             const std::vector<std::vector<long long>> &second,
                                             int n, int mod) {
  std::vector<std::vector<long long>> result(n, std::vector<long long>(n));
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      result[i][j] = 0;
    }
  }
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      for (size_t k = 0; k < n; ++k) {
        result[i][j] += first[i][k] * second[k][j];
        result[i][j] %= mod;
      }
    }
  }
  return result;
}
std::vector<std::vector<long long>> exponentiation(const std::vector<std::vector<long long>> &first, const BigInteger &n, int mod) {
  std::string binary = BigInteger::convert_to_binary(n);
  int size = first[0].size();
  std::vector<std::vector<long long>> result(size, std::vector<long long>(size));
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (i == j) {
        result[i][j] = 1;
      } else {
        result[i][j] = 0;
      }
    }
  }
  std::vector<std::vector<long long>> current_binary = first;
  for (char i : binary) {
    if (i == '1') {
      result = multiply(result, current_binary, size, mod);
    }
    current_binary = multiply(current_binary, current_binary, size, mod);
  }
  return result;
}

int main() {
  BigInteger n;
  int mod = 999999937;
  std::cin >> n;
  std::vector<std::vector<long long>> matrix(5, std::vector<long long>(5));
  while (!(n == 0)) {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        matrix[i][j] = 1;
      }
    }
    matrix[3][2] = 0;
    matrix[3][4] = 0;
    matrix[4][2] = 0;
    matrix[4][4] = 0;
    matrix = exponentiation(matrix, n - 1, mod);
    long long sum = 0;
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        sum += matrix[i][j];
      }
    }
    sum %= mod;
    std::cout << sum << '\n';
    std::cin >> n;
  }

  return 0;
}