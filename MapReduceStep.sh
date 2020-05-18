
# "$1" has input file
# "$2" has output file

head "$1"
./scraper.py "$1" temp.txt
head temp.txt
./ttb temp.txt temp.bin
head temp.bin
./sort temp.bin temp_sorted.bin
head temp_sorted.bin
./btt temp_sorted.bin "$2"
head "$2"
