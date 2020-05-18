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
  std::string line;
  while (getline(fin, line)) {
    assert(line.size() <= MAX_LINK_LENGTH && !line.empty());
    line.append(std::string(MAX_LINK_LENGTH - line.size(), ' '));
    assert(line.size() == MAX_LINK_LENGTH);
    fwrite(&line[0], 1, MAX_LINK_LENGTH, out);
  }
}