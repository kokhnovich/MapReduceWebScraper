#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cassert>

#include "constants.h"

const int BLOCK = 20;
std::string INPUT;
std::string OUTPUT;
const std::string TEMP = "tmp.bin";
const int line_size = MAX_LINK_LENGTH;

std::vector<std::string> ReadSubarray(FILE* file, int start, int count) {
  std::vector<std::string> ans(count, std::string(line_size, 0));
  fseek(file, start * line_size, SEEK_SET);
  for (auto& i : ans) {
    fread(&i[0], line_size, 1, file);
  }
  return ans;
}

class CacheManager {
 public:
  explicit CacheManager(FILE* temp) : tmp(temp), cache_(BLOCK) {}

  void AddElement(std::string element) {
    if (size_ == max_size_) {
      Write();
    }
    cache_[size_++] = std::move(element);
  }
  void Write() {
    fseek(tmp, BLOCK * line_size * number_of_blocks_inside_tmp, SEEK_SET);
    for (int i = 0; i < size_; ++i) {
      fwrite(&cache_[i][0], 1, line_size, tmp);
    }
    ++number_of_blocks_inside_tmp;
    size_ = 0;
  }
  
  void CopyToFile(FILE* file, int start) {
    fseek(tmp, 0, SEEK_SET);
    fseek(file, start * line_size, SEEK_SET);
    for (int i = 0; i < number_of_blocks_inside_tmp; ++i) {
      std::vector<std::string> temp(BLOCK, std::string(line_size, 0));
      for (auto& elem : temp) {
        fread(&elem[0], 1, line_size, tmp);
      }
      for (auto& elem : temp) {
        fwrite(elem.c_str(), 1, elem.size(), file);
      }
    }
    for (int i = 0; i < size_; ++i) {
      fwrite(&cache_[i][0], 1, line_size, file);
    }
    fseek(file, 0, SEEK_SET);
    fseek(tmp, 0, SEEK_SET);
  }
 private:
  FILE* tmp;
  int number_of_blocks_inside_tmp = 0;
  std::vector<std::string> cache_;
  int size_ = 0;
  int max_size_ = BLOCK;
};

class StorageManager {
 public:
  StorageManager(FILE* file, int start_index, int sz) : start_index_(start_index),
                                                        number_of_elements_(sz),
                                                        current_index_(0),
                                                        file_(file),
                                                        block_(BLOCK) {
    UpdateChunk();
  }

  [[nodiscard]] bool HasReachedEnd() const {
    return current_index_ == number_of_elements_;
  }

  [[nodiscard]] std::string SeeNextElement() const {
    assert(!HasReachedEnd());
    return block_[current_index_ % BLOCK];
  }

  std::string GetNextElement() {
    assert(!HasReachedEnd());
    std::string to_return = SeeNextElement();
    ++current_index_;
    if (current_index_ % BLOCK == 0) {
      UpdateChunk();
    }
    return to_return;
  }

 private:
  void UpdateChunk() {
    if (number_of_elements_ - current_index_ == 0) return;
    block_ = ReadSubarray(file_, start_index_ + current_index_, std::min(BLOCK, number_of_elements_ - current_index_));
  }

  std::vector<std::string> block_;
  FILE* file_;
  int start_index_;
  int number_of_elements_;
  int current_index_;
};

void MergeSort(FILE* file, FILE* tmp, int left_block_index, int right_block_index, int n) {
  if (left_block_index == right_block_index) return;
  int mid = (left_block_index + right_block_index) / 2;

  MergeSort(file, tmp, left_block_index, mid, n);
  MergeSort(file, tmp, mid + 1, right_block_index, n);

  int left_start = left_block_index * BLOCK;
  int right_start = (mid + 1) * BLOCK;

  int left_length = right_start - left_start;
  int right_length = std::min(n - right_start, (right_block_index + 1) * BLOCK - right_start);
  StorageManager left(file, left_start, left_length);
  StorageManager right(file, right_start, right_length);
  CacheManager cache(tmp);
  while (!left.HasReachedEnd() || !right.HasReachedEnd()) {
    if (left.HasReachedEnd()) {
      cache.AddElement(right.GetNextElement());
    } else if (right.HasReachedEnd()) {
      cache.AddElement(left.GetNextElement());
    } else if (left.SeeNextElement() < right.SeeNextElement()) {
      cache.AddElement(left.GetNextElement());
    } else {
      cache.AddElement(right.GetNextElement());
    }
  }
  cache.CopyToFile(file, left_start);
}

void Solve() {
  FILE* in = fopen(INPUT.c_str(), "rb");
  FILE* out = fopen(OUTPUT.c_str(), "wb+");
  FILE* tmp = fopen(TEMP.c_str(), "wb+");

  int n = (int) std::filesystem::file_size(INPUT) / MAX_LINK_LENGTH;

  for (int i = 0; i < n; i += BLOCK) {
    int len = std::min(BLOCK, n - i);
    std::vector<std::string> a(len, std::string(line_size, 0));
    for (auto& elem : a) {
      fread(&elem[0], 1, line_size, in);
    }
    std::sort(a.begin(), a.end());
    for (auto& elem : a) {
      fwrite(elem.c_str(), 1, elem.size(), out);
    }
  }

  int cnt_blocks = (n + BLOCK - 1) / BLOCK;
  MergeSort(out, tmp, 0, cnt_blocks - 1, n);
  fclose(in);
  fclose(out);
  remove(TEMP.c_str());
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Script requires 2 args: input and output binary files" << std::endl;
    return 1;
  }
  INPUT = argv[1];
  OUTPUT = argv[2];
  Solve();
  return 0;
}