#include <bits/stdc++.h>
#include <boost/process.hpp>

#include "Constants.h"
#include "ExternalMergeSort.cpp"

namespace bp = boost::process;

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
  int code = bp::system("cat " + list_of_all_output_files,
                        bp::std_out > info.output_path,
                        bp::std_err > stderr);
  if (code != 0) {
    std::cerr << "error while cat files" << std::endl;
    exit(code);
  }
  RemoveFiles(input_files);
  RemoveFiles(output_files);
  exit(0);
}

void Reduce(const MainInfo& info) {
  std::string temp_input = "reduce_temp_input.txt";
  std::filesystem::copy_file(info.input_path, temp_input);
  ExtMergeSort::Run(temp_input);

  std::vector<std::string> input_files;
  std::vector<std::string> output_files;
  std::vector<bp::child> child_processes;
  std::vector<std::string> block;
  std::string line, prev_line;
  std::ifstream fin(temp_input);
  int temp_file_count = 0;
  bool is_first = true;
  while (getline(fin, line, '\t')) {
    std::string value;
    getline(fin, value);
    if (line == prev_line || is_first) {
      if (is_first) {
        is_first = false;
        prev_line = line;
      }
      block.push_back(line.append("\t").append(value));
    } else {
      prev_line = line;
      auto input_file = GetFileName("input", temp_file_count);
      auto output_file = GetFileName("output", temp_file_count);
      ++temp_file_count;
      std::ofstream fout(input_file);
      for (auto& i : block) {
        fout << i << std::endl;
      }
      input_files.push_back(input_file);
      output_files.push_back(output_file);
      child_processes.emplace_back(info.script_path,
                                   bp::std_out > output_file,
                                   bp::std_err > stderr,
                                   bp::std_in < input_file);
      block.clear();
      block.push_back(line.append("\t").append(value));
    }
  }
  if (!block.empty()) {
    auto input_file = GetFileName("input", temp_file_count);
    auto output_file = GetFileName("output", temp_file_count);
    std::ofstream fout(input_file);
    for (auto& i : block) {
      fout << i << std::endl;
    }
    input_files.push_back(input_file);
    output_files.push_back(output_file);
    child_processes.emplace_back(info.script_path,
                                 bp::std_out > output_file,
                                 bp::std_err > stderr,
                                 bp::std_in < input_file);
    block.clear();
  }

  for (auto& child : child_processes) {
    child.wait();
  }

  RemoveFiles(input_files);

  { // output data
    std::ofstream stream(info.output_path);
    for (auto& name : output_files) {
      std::ifstream in(name);
      std::string word;
      int count;
      while (in >> word >> count) {
        stream << word << '\t' << count << std::endl;
      }
    }
  }
  remove(temp_input.c_str());
  RemoveFiles(output_files);
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
