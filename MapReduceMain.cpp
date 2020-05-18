#include <bits/stdc++.h>
#include <boost/process.hpp>

#include "constants.h"

namespace bp = boost::process;

int RemoveFile(const std::string& file_name) {
  return remove(file_name.c_str());
}

void RemoveFiles(const std::vector<std::string>& file_names) {
  for (const auto& name: file_names) {
    int code = remove(name.c_str());
    if (code != 0) {
      std::cerr << "error while removing file" << std::endl;
      exit(code);
    }
  }
}

std::string GetFileName(const std::string& type, int name) {
  return "tmp_file_" + type + "_" + std::to_string(name) + ".txt";
}

struct MainInfo {
  std::string mode;
  std::string script_path;
  std::string input_path;
  std::string output_path;
};

// returns input and output values respectably
std::pair<std::string, std::string> RunMapJob(std::vector<bp::child>& child_processes,
                                              const std::string& script_path,
                                              int index) {
  std::string input_file = GetFileName("input", index);
  std::string output_file = GetFileName("output", index);

  child_processes.emplace_back(script_path,
                               bp::std_out > output_file,
                               bp::std_err > stderr,
                               bp::std_in < input_file);
  return std::make_pair(input_file, output_file);
}

void Map(const MainInfo& info) {
  std::vector<std::string> input_files;
  std::vector<std::string> output_files;
  std::vector<bp::child> child_processes;
  std::string list_of_all_output_files;

  std::ifstream in(info.input_path);
  std::vector<std::string> local_input;
  local_input.reserve(LENGTH_TO_DIVIDE_MAP);
  std::string line;
  while (getline(in, line)) {
    local_input.push_back(line);
    if (local_input.size() == 10) {
      auto io_files = RunMapJob(child_processes, info.script_path, input_files.size());
      input_files.push_back(io_files.first);
      output_files.push_back(io_files.second);
      list_of_all_output_files.append(io_files.second).append(" ");
      local_input.clear();
    }
  }
  if (!local_input.empty()) {
    auto io_files = RunMapJob(child_processes, info.script_path, input_files.size());
    input_files.push_back(io_files.first);
    output_files.push_back(io_files.second);
    list_of_all_output_files.append(io_files.second).append(" ");
    local_input.clear();
  }
  for (auto& child : child_processes) {
    child.wait();
  }
  int code = bp::system("cat " + list_of_all_output_files, bp::std_out > info.output_path, bp::std_err > stderr);
  if (code != 0) {
    std::cerr << "error while cat files" << std::endl;
    exit(code);
  }
  RemoveFiles(input_files);
  RemoveFiles(output_files);
}

void Reduce(const MainInfo& info) {
  std::vector<std::pair<std::string, int>> words;
  { // input data
    std::string word;
    int cnt;
    std::ifstream stream(info.input_path);
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
    child_processes.emplace_back(info.script_path,
                                 bp::std_out > output_file,
                                 bp::std_err > stderr,
                                 bp::std_in < input_file);
  }

  for (auto& child : child_processes) {
    child.wait();
  }

  RemoveFiles(input_files);

  { // output data
    std::ofstream stream(info.output_path);
    for (const auto& name : output_files) {
      std::ifstream in(name);
      std::string word;
      int count;
      while (in >> word >> count) {
        stream << word << ' ' << count << std::endl;
      }
    }
  }
  RemoveFiles(output_files);
};

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Script requires 4 arguments" << std::endl;
    return 0;
  }
  MainInfo main_info{argv[1], argv[2], argv[3], argv[4]};
  std::string mode = argv[1];
  std::string script_path = argv[2];
  std::string input_path = argv[3];
  std::string output_path = argv[4];

  if (mode == "map") {
    Map(main_info);
  } else if (mode == "reduce") {
    Reduce(main_info);
  } else {
    std::cerr << R"(mode can be only "map" or "reduce")" << std::endl;
  }
}
