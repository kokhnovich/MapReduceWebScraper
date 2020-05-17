#include <bits/stdc++.h>
#include <boost/process.hpp>

namespace bp = boost::process;

int RemoveFile(const std::string& file_name) {
  return remove(file_name.c_str());
}

std::string GetFileName(const std::string& type, int name) {
  return "tmp_file_" + type + "_" + std::to_string(name) + ".txt";
}

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Script requires 4 arguments" << std::endl;
  }
  std::string mode = argv[1];
  std::string script_path = argv[2];
  std::string input_path = argv[3];
  std::string output_path = argv[4];

  if (mode == "map") {
    bp::system(script_path, bp::std_out > output_path, bp::std_err > stderr, bp::std_in < input_path);
  } else if (mode == "reduce") {
    std::vector<std::pair<std::string, int>> words;
    { // input data
      std::string word;
      int cnt;
      std::ifstream stream(input_path);
      while (stream >> word >> cnt) {
        words.emplace_back(word, cnt);
      }
    }

    std::sort(words.begin(), words.end());

    std::vector<std::string> input_files;
    std::vector<std::string> output_files;
    std::vector<bp::child> child_processes;
    for (int i = 0; i < words.size();) {
      std::string input_file = GetFileName("input", i);
      std::string output_file = GetFileName("output", i);

      input_files.push_back(input_file);
      output_files.push_back(output_file);

      std::ofstream os(input_file);
      int first_index = i;
      while (i < words.size() && words[i].first == words[first_index].first) {
        os << words[i].first << ' ' << words[i].second << std::endl;
        ++i;
      }
      child_processes.emplace_back(script_path,
                                   bp::std_out > output_file,
                                   bp::std_err > stderr,
                                   bp::std_in < input_file);
    }

    for (auto& child : child_processes) {
      child.wait();
    }

    for (const auto& file_name : input_files) {
      int code = RemoveFile(file_name);
      if (code != 0) {
        std::cerr << "cannot remove file " << file_name << std::endl;
        return code;
      }
    }

    { // output data
      std::ofstream stream(output_path);
      for (auto& name : output_files) {
        std::ifstream in(name);
        std::string word;
        int count;
        while (in >> word >> count) {
          stream << word << ' ' << count << std::endl;
        }
      }

      for (const auto& file_name : output_files) {
        int code = RemoveFile(file_name);
        if (code != 0) {
          std::cerr << "cannot remove file " << file_name << std::endl;
          return code;
        }
      }
    }
  } else {
    std::cerr << R"(mode can be only "map" or "reduce")" << std::endl;
  }
}
