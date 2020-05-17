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

  FILE* in = fopen(input_file.c_str(), "rb");
  std::ofstream fout(output_file);

  long long number_of_lines;
  fread(&number_of_lines, sizeof(long long), 1, in);
  fout << number_of_lines << std::endl;
  while (number_of_lines--) {
    std::string line(MAX_LINK_LENGTH, 0);
    fread(&line[0], 1, MAX_LINK_LENGTH, in);
    fout << line << std::endl;
  }
}