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
std::pair<std::string, std::string> RunMapJob(const std::vector<std::string>& local_input,
                                              std::vector<bp::child>& child_processes,
                                              const std::string& script_path,
                                              int index) {
  std::string input_file = GetFileName("input", index);
  std::string output_file = GetFileName("output", index);

  std::ofstream fout(input_file);
  for (const auto& line : local_input) {
    fout << line << std::endl;
  }

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

  std::cout << info.input_path << std::endl;
  std::ifstream in(info.input_path);
  std::vector<std::string> local_input;
  local_input.reserve(LENGTH_TO_DIVIDE_MAP);
  std::string line;
  while (getline(in, line)) {
    local_input.push_back(line);
    if (local_input.size() == LENGTH_TO_DIVIDE_MAP) {
      auto io_files = RunMapJob(local_input, child_processes, info.script_path, input_files.size());
      input_files.push_back(io_files.first);
      output_files.push_back(io_files.second);
      list_of_all_output_files.append(io_files.second).append(" ");
      local_input.clear();
    }
  }
  if (!local_input.empty()) {
    auto io_files = RunMapJob(local_input, child_processes, info.script_path, input_files.size());
    input_files.push_back(io_files.first);
    output_files.push_back(io_files.second);
    list_of_all_output_files.append(io_files.second).append(" ");
    local_input.clear();
  }
  for (auto& child : child_processes) {
    child.wait();
  }
  assert(!child_processes.empty());
  std::cout << child_processes.size() << std::endl;
  std::cout << list_of_all_output_files << std::endl;
  int code = bp::system("cat " + list_of_all_output_files, bp::std_out > info.output_path, bp::std_err > stderr);
  if (code != 0) {
    std::cerr << "error while cat files" << std::endl;
    exit(code);
  }
  RemoveFiles(input_files);
  RemoveFiles(output_files);
  exit(0);
}

void Reduce(const MainInfo& info) {
  if (bp::system("bash SortTxtFile.sh " + info.input_path) != 0) {
    std::cerr << "error while sort" << std::endl;
    return;
  }
  int code = bp::system(info.script_path,
                        bp::std_out > info.output_path,
                        bp::std_err > stderr,
                        bp::std_in < info.input_path);
  if (code != 0) {
    std::cerr << "error while reduce" << std::endl;
    exit(code);
  }
};

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Script requires 4 arguments" << std::endl;
    return 0;
  }
  MainInfo main_info{argv[1], argv[2], argv[3], argv[4]};
  if (main_info.mode == "map") {
    Map(main_info);
  } else if (main_info.mode == "reduce") {
    Reduce(main_info);
  } else {
    std::cerr << R"(mode can be only "map" or "reduce")" << std::endl;
  }
}
