depth="$3"

cat "$1" > last_output.txt
while [[ "$depth" -gt 0 ]]; do
  echo "running map..." &&
  time ./mr map ./Scraper.py last_output.txt temp.txt &&

  echo "running reduce..." &&
  time ./mr reduce ./reducer temp.txt last_output.txt &&

  echo "checking..." &&
  wc -l last_output.txt &&
  rm temp.txt &&
  ((depth--))
done
cat last_output.txt > "$2"
rm last_output.txt