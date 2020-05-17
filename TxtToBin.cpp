#include <iostream>
#include <fstream>
#include <cassert>

#include "constants.h"

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
  long long number_of_lines;
  fin >> number_of_lines;
  fin.ignore();
  fwrite(&number_of_lines, sizeof(long long), 1, out);
  while (number_of_lines--) {
    std::string line;
    getline(fin, line);
    assert(line.size() <= MAX_LINK_LENGTH);
    line = std::string(MAX_LINK_LENGTH - line.length(), ' ').append(line);
    assert(line.size() == MAX_LINK_LENGTH);
    fwrite(&line[0], 1, MAX_LINK_LENGTH, out);
  }
}