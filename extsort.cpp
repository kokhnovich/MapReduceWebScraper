#include <bits/stdc++.h>

// #define int long long
#define ll long long

using namespace std;

const int BLOCK = 3;
const string INPUT = "input.bin";
const string OUTPUT = "output.bin";
const string TEMP = "tmp.bin";
const int line_size = 4;

void Check(const string& file_to_check) {
  cout << "checking " << file_to_check << endl;
  FILE* out = fopen(file_to_check.c_str(), "rb");
  ll n;
  fread(&n, sizeof(ll), 1, out);
  cout << n << endl;
  vector<string> a(n, string(line_size, 0));
  for (auto& i : a) {
    fread(&i[0], 1, line_size, out);
  }
  for (auto& i : a) {
    cout << i << " ";
  }
  cout << endl;
  sort(a.begin(), a.begin() + n);
  for (auto& i : a) {
    cout << i << " ";
  }
  cout << endl;
}

void FillBinFile() {
  FILE* in = fopen(INPUT.c_str(), "wb+");
  ll n = 10;
  vector<string> a(n);
  for (int i = 0; i < n; ++i) {
    a[i] = string(line_size, 'k');
    a[i][0] = (char) ('9' - i);
    cout << a[i] << " ";
  }
  cout << endl;

  fwrite(&n, sizeof(n), 1, in);
  for (auto& i : a) {
    fwrite(i.c_str(), 1, i.size(), in);
  }
  fclose(in);
}

void ReadSubrray(FILE* file, int start, int count, ll* block_) {
  //ll a[count];
  fseek(file, 8 + start * 8, SEEK_SET);
  fread(block_, sizeof(ll), count, file);
  //vector<ll> arr(count);
  //for (int i = 0; i < count; ++i) {
  //  arr[i] = a[i];
  //}
  // assert(arr.size() == count);
  // return arr;
}

class CacheManager {
 public:
  explicit CacheManager(FILE* temp) : tmp(temp), cache_(BLOCK) {}

  void AddElement(ll num) {
    if (size_ == max_size_) {
      Write();
    }
    cache_[size_++] = num;
  }
  void Write() {
    fseek(tmp, BLOCK * 8 * number_of_blocks_inside_tmp, SEEK_SET);
    //ll arr[size_];

    //cout << "test" << number_of_blocks_inside_tmp << endl;
    //for (int i = 0; i < cache_.size(); ++i) {
    //arr[i] = cache_[i];
    //cout << arr[i] << " ";
    //}
    //cout << endl;

    fwrite(&cache_[0], sizeof(ll), size_, tmp);
    ++number_of_blocks_inside_tmp;
    size_ = 0;
  }
  /// The class isn't usable after this function.
  void CopyToFile(FILE* file, int start, int expected_length) {
    // Write();
    // assert(number_of_blocks_inside_tmp*BLOCK+cache_.size() == expected_length);
    fseek(tmp, 0, SEEK_SET);
    fseek(file, 8 + start * 8, SEEK_SET);
    for (int i = 0; i < number_of_blocks_inside_tmp; ++i) {
      ll temp[BLOCK];
      fread(temp, sizeof(ll), BLOCK, tmp);
      fwrite(temp, sizeof(ll), BLOCK, file);
    }
    //ll arr[cache_.size()];
    //for (int i = 0; i < cache_.size(); ++i) {
    //  arr[i] = cache_[i];
    //}
    fwrite(&cache_[0], sizeof(ll), size_, file);
    fseek(file, 8, SEEK_SET);
    fseek(tmp, 0, SEEK_SET);
  }
 private:
  FILE* tmp;
  int number_of_blocks_inside_tmp = 0;
  vector<ll> cache_;
  int size_ = 0;
  int max_size_ = BLOCK;
};

class StorageManager {
 public:
  StorageManager(FILE* file, int start_index, int sz) : start_index_(start_index),
                                                        number_of_elements_(sz),
                                                        current_index_(0),
                                                        file_(file) {
    UpdateChunk();
  }

  [[nodiscard]] bool HasReachedEnd() const {
    return current_index_ == number_of_elements_;
  }

  [[nodiscard]] ll SeeNextElement() const {
    assert(!HasReachedEnd());
    return block_[current_index_ % BLOCK];
  }

  ll GetNextElement() {
    assert(!HasReachedEnd());
    ll to_return = SeeNextElement();
    ++current_index_;
    if (current_index_ % BLOCK == 0) {
      UpdateChunk();
    }
    return to_return;
  }

 private:
  void UpdateChunk() {
    if (number_of_elements_ - current_index_ == 0) return;
    ReadSubrray(file_, start_index_ + current_index_, min(BLOCK, number_of_elements_ - current_index_), block_);
    return;
  }

  ll block_[BLOCK];
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
  int right_length = min(n - right_start, (right_block_index + 1) * BLOCK - right_start);
  StorageManager left(file, left_start, left_length);
  StorageManager right(file, right_start, right_length);
  CacheManager cache(tmp);
  while (!left.HasReachedEnd() || !right.HasReachedEnd()) {
    if (left.HasReachedEnd()) {
      cache.AddElement(right.GetNextElement());
    } else if (right.HasReachedEnd()) {
      cache.AddElement(left.GetNextElement());
    } else if (left.SeeNextElement() < right.SeeNextElement()) {
      //cout << left.SeeNextElement() << " " <<  right.SeeNextElement() << endl;
      cache.AddElement(left.GetNextElement());
    } else {
      //cout << left.SeeNextElement() << " " <<  right.SeeNextElement() << endl;
      cache.AddElement(right.GetNextElement());
    }
  }
  cache.CopyToFile(file, left_start, left_length + right_length);
}

void Solve() {
  FILE* in;
  FILE* out;
  FILE* tmp;
  in = fopen("input.bin", "rb");
  out = fopen("output.bin", "wb+");
  tmp = fopen("tmp.bin", "wb+");

  int n;
  fread(&n, sizeof(ll), 1, in);
  fwrite(&n, sizeof(ll), 1, out);

  for (int i = 0; i < n; i += BLOCK) {
    int len = min(BLOCK, n - i);
    ll a[len];
    fread(a, sizeof(ll), len, in);
    sort(a, a + len);
    fwrite(a, sizeof(ll), len, out);
  }

  int cnt_blocks = (n + BLOCK - 1) / BLOCK;
  MergeSort(out, tmp, 0, cnt_blocks - 1, n);
  fclose(in);
  fclose(out);
}

int32_t main() {
  FillBinFile();
  Solve();
  Check(INPUT);
  Check(OUTPUT);
  return 0;
}