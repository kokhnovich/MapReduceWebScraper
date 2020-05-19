bash sort "$1" sort_bin_file_temp.bin
cat sort_bin_file_temp.bin > "$1"
rm sort_bin_file_temp.bin