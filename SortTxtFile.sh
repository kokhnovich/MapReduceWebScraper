# transforms .txt to .bin
# sorts it
# transforms .bin to .txt

./ttb "$1" sort_temp.bin &&
./sort sort_temp.bin sort_temp_sorted.bin &&
./btt sort_temp_sorted.bin "$1"
rm sort_temp_sorted.bin sort_temp.bin