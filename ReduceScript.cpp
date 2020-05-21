#include <iostream>

int main() {
  std::string key, ans;
  int res = 1;
  while (std::getline(std::cin, key, '\t')) {
    std::string value;
    std::getline(std::cin, value);
    if (ans.empty()) ans = key;
    // if there is at least one zero, then
    // the link should be marked as visited.
    res &= (value == "1" ? 1 : 0);
  }
  std::cout << ans << '\t' << res << std::endl;
  return 0;
}
