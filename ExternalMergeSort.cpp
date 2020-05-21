#include <bits/stdc++.h>

using namespace std;

string file = "input.txt";
string temp = "merge_sort_temp.txt";
int number_of_blocks;

int BLOCK_SIZE = 5;

string GetTempFileName(int index) {
  return "merge_sort_temp_file_" + to_string(index);
}

void WriteBlockToTempFile(const vector<string>& block, int index) {
  assert(block.size() <= BLOCK_SIZE);
  ofstream fout(GetTempFileName(index));
  for (const auto& i : block) {
    fout << i << endl;
  }
}

vector<string> GetBlockFromStream(ifstream& fin) {
  vector<string> ans;
  string line;
  while (ans.size() < BLOCK_SIZE && getline(fin, line)) {
    ans.push_back(line);
  }
  assert(ans.size() <= BLOCK_SIZE);
  return ans;
}

vector<string> GetBlock(int index) {
  ifstream fin(GetTempFileName(index));
  return GetBlockFromStream(fin);
}

class WriteManager {
 public:
  WriteManager() : fout(temp) {}
  ~WriteManager() {
    fout.close();
    remove(temp.c_str());
  }
  void Write(const string& str) {
    fout << str << endl;
  }
  void Flush(int first_block, int block_count) {
    fout.flush();
    fout.close();
    vector<string> block;
    block.reserve(BLOCK_SIZE);
    string line;
    ifstream fin(temp);

    for (int index = first_block; index < first_block + block_count; ++index) {
      block = GetBlockFromStream(fin);
      WriteBlockToTempFile(block, index);
    }
  }
 private:
  ofstream fout;
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
    string to_return = current_string;
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
  ifstream fin;
  string current_string;
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
  ifstream fin(file);
  string line;
  vector<string> block;
  while (getline(fin, line)) {
    block.push_back(line);
    if (block.size() == BLOCK_SIZE) {
      sort(block.begin(), block.end());
      ofstream fout(GetTempFileName(number_of_blocks));
      ++number_of_blocks;
      for (const auto& i : block) {
        fout << i << endl;
      }
      block.clear();
    }
  }
  if (!block.empty()) {
    sort(block.begin(), block.end());
    ofstream fout(GetTempFileName(number_of_blocks));
    ++number_of_blocks;
    for (const auto& i : block) {
      fout << i << endl;
    }
    block.clear();
  }
}

void MergeTempFiles() {
  remove(file.c_str());
  ofstream fout(file);
  for (int i = 0; i < number_of_blocks; ++i) {
    auto block = GetBlock(i);
    for (const auto& j : block) {
      fout << j << endl;
    }
  }
}

void ClearWorkDir() {
  remove(temp.c_str());
  for (int i = 0; i < number_of_blocks; ++i) {
    remove(GetTempFileName(i).c_str());
  }
}

int main() {
  SplitToTempFiles();
  MergeSort(0, number_of_blocks - 1);
  MergeTempFiles();
  ClearWorkDir();
  return 0;
}