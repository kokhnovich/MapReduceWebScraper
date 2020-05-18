#include <iostream>
#include <fstream>
#include <cassert>
#include <filesystem>

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
  int n = std::filesystem::file_size(input_file) / MAX_LINK_LENGTH;
  while (n--) {
    std::string line(MAX_LINK_LENGTH, 0);
    fread(&line[0], 1, MAX_LINK_LENGTH, in);
    auto first_space = line.find_first_of(' ');
    line = line.substr(0, first_space);
    fout << line << std::endl;
  }
}