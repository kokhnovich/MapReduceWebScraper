#include <iostream>

int main() {
  std::string word;
  int res = 0, count;
  while (std::cin >> word >> count) {
    res += count;
  }

  std::cout << word << '\t' << res << std::endl;
  return 0;
}
