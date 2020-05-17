#include <iostream>
#include <fstream>
#include <cassert>

// @TODO format to Key\tValue

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Requires two arguments: source text file and destination binary file name" << std::endl;
    return 1;
  }

  std::string input_file = argv[1];
  std::string output_file = argv[2];

  std::ifstream fin(input_file);
  FILE* out = fopen(output_file.c_str(), "wb+");
  long long number_of_lines, max_length;
  fin >> number_of_lines >> max_length;
  fin.ignore();
  fwrite(&number_of_lines, sizeof(long long), 1, out);
  while (number_of_lines--) {
    std::string line;
    getline(fin, line);
    assert(line.size() <= max_length);
    line = std::string(max_length - line.length(), ' ').append(line);
    assert(line.size() == max_length);
    fwrite(&line[0], 1, max_length, out);
  }
}