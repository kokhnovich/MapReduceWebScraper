# $1 - depth
depth="$1"

while [[ "$depth" -gt 0 ]]; do
  echo "running map..." &&
  time ./mr map ./scraper.py input.txt temp.txt &&

  echo "running reduce..." &&
  time ./mr reduce ./reducer temp.txt output.txt &&

  echo "checking..." &&

  wc -l output.txt &&
  rm temp.txt &&
  cat output.txt > input.txt
  ((depth--))
done