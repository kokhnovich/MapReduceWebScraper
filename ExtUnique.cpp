#include <iostream>
#include <string>

int main(int argc, char** argv) {
  std::string prev_key, now_key, now_value;
  while (getline(std::cin, now_key, '\t')) {
    getline(std::cin, now_value, '\n');
    if (now_key != prev_key) {
      std::cout << now_key << '\t' << now_value << std::endl;
      prev_key = now_key;
    }
  }
  std::cout.flush();
}