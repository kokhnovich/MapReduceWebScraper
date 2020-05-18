
# "$1" has input file
# "$2" has output file

./scraper.py "$1" temp.txt
./ttb temp.txt temp.bin
./sort temp.bin temp_sorted.bin
./btt temp_sorted.bin "$2"
rm temp.txt temp.bin temp_sorted.bin