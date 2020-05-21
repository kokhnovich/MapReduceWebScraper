#include <bits/stdc++.h>

namespace ExtMergeSort {

std::string file = "input.txt";
std::string temp = "merge_sort_temp.txt";

int number_of_blocks;

int BLOCK_SIZE = 5;

std::string GetTempFileName(int index) {
  return "merge_sort_temp_file_" + std::to_string(index);
}

void WriteBlockToTempFile(const std::vector<std::string>& block, int index) {
  assert(block.size() <= BLOCK_SIZE);
  std::ofstream fout(GetTempFileName(index));
  for (const auto& i : block) {
    fout << i << std::endl;
  }
}

std::vector<std::string> GetBlockFromStream(std::ifstream& fin) {
  std::vector<std::string> ans;
  std::string line;
  while (ans.size() < BLOCK_SIZE && getline(fin, line)) {
    ans.push_back(line);
  }
  assert(ans.size() <= BLOCK_SIZE);
  return ans;
}

std::vector<std::string> GetBlock(int index) {
  std::ifstream fin(GetTempFileName(index));
  return GetBlockFromStream(fin);
}

class WriteManager {
 public:
  WriteManager() : fout(temp) {}
  ~WriteManager() {
    fout.close();
    remove(temp.c_str());
  }
  void Write(const std::string& str) {
    fout << str << std::endl;
  }
  void Flush(int first_block, int block_count) {
    fout.flush();
    fout.close();
    std::vector<std::string> block;
    block.reserve(BLOCK_SIZE);
    std::string line;
    std::ifstream fin(temp);

    for (int index = first_block; index < first_block + block_count; ++index) {
      block = GetBlockFromStream(fin);
      WriteBlockToTempFile(block, index);
    }
  }
 private:
  std::ofstream fout;
};

class ReadManager {
 public:
  ReadManager(int first_block, int count_of_block)
      : first_block_index(first_block), number_of_blocks(count_of_block) {
    OpenNextBlock();
  }

  [[nodiscard]] bool HasReachedEnd() const {
    return is_end;
  }

  [[nodiscard]] std::string SeeNextElement() {
    assert(!HasReachedEnd());
    return current_string;
  }

  std::string GetNextElement() {
    assert(!HasReachedEnd());
    std::string to_return = current_string;
    getline(fin, current_string);
    if (IsBlockEnded()) {
      OpenNextBlock();
    }
    return to_return;
  }

 private:
  bool IsBlockEnded() {
    return current_string.empty() && fin.peek() == EOF;
  }

  void OpenNextBlock() {
    if (fin.is_open()) {
      fin.close();
    }
    if (counter_of_blocks == number_of_blocks) {
      is_end = true;
      return;
    }
    fin.open(GetTempFileName(first_block_index + counter_of_blocks));
    ++counter_of_blocks;
    getline(fin, current_string);
  }

  bool is_end = false;
  std::ifstream fin;
  std::string current_string;
  int first_block_index;
  int number_of_blocks;
  int counter_of_blocks = 0;
};

void MergeSort(int l, int r) {
  if (l == r) return;
  int m = (l + r) / 2;

  MergeSort(l, m);
  MergeSort(m + 1, r);

  WriteManager writer;
  ReadManager left(l, m - l + 1);
  ReadManager right(m + 1, r - m);
  while (!left.HasReachedEnd() || !right.HasReachedEnd()) {
    if (left.HasReachedEnd()) {
      writer.Write(right.GetNextElement());
    } else if (right.HasReachedEnd()) {
      writer.Write(left.GetNextElement());
    } else if (left.SeeNextElement() < right.SeeNextElement()) {
      writer.Write(left.GetNextElement());
    } else {
      writer.Write(right.GetNextElement());
    }
  }
  writer.Flush(l, r - l + 1);
}

void SplitToTempFiles() {
  std::ifstream fin(file);
  std::string line;
  std::vector<std::string> block;
  while (getline(fin, line)) {
    block.push_back(line);
    if (block.size() == BLOCK_SIZE) {
      sort(block.begin(), block.end());
      std::ofstream fout(GetTempFileName(number_of_blocks));
      ++number_of_blocks;
      for (const auto& i : block) {
        fout << i << std::endl;
      }
      block.clear();
    }
  }
  if (!block.empty()) {
    sort(block.begin(), block.end());
    std::ofstream fout(GetTempFileName(number_of_blocks));
    ++number_of_blocks;
    for (const auto& i : block) {
      fout << i << std::endl;
    }
    block.clear();
  }
}

void MergeTempFiles() {
  remove(file.c_str());
  std::ofstream fout(file);
  for (int i = 0; i < number_of_blocks; ++i) {
    auto block = GetBlock(i);
    for (const auto& j : block) {
      fout << j << std::endl;
    }
  }
}

void ClearWorkDir() {
  remove(temp.c_str());
  for (int i = 0; i < number_of_blocks; ++i) {
    remove(GetTempFileName(i).c_str());
  }
}

void Run(const std::string& file_to_sort) {
  file = file_to_sort;
  SplitToTempFiles();
  MergeSort(0, number_of_blocks - 1);
  MergeTempFiles();
  ClearWorkDir();
}

}