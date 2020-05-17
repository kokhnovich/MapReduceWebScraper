#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

// @TODO format to Key\tValue

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Requires two arguments: source text file and destination binary file name" << std::endl;
    return 1;
  }

  std::string input_file = argv[1];
  std::string output_file = argv[2];

  std::ifstream fin(input_file);
  std::ofstream fout(output_file);

  long long n;
  fin >> n;
  fin.ignore();
  fout << n << std::endl;
  std::string prev_line;
  for (int i = 0; i < n; ++i) {
    std::string line;
    getline(fin, line);
    if (i == 0 || line != prev_line) {
      fout << line << std::endl;
      prev_line = line;
    }
  }
}