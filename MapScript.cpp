#include <iostream>
#include <sstream>

int main() {
  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    std::string key;
    getline(iss, key, '\t');

    std::string word;
    while (iss >> word) {
      std::cout << word << '\t' << 1 << std::endl;
    }
  }
  return 0;
}
