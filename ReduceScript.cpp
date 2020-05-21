#include <iostream>

int main() {
  std::string key;
  int res = 1;
  while (getline(cin, key, '\t')) {
    std::string value;
    getline(cin, value);
    res &= (value == "1" ? 1 : 0);
  }
  std::cout << key << '\t' << res << std::endl;
  return 0;
}
