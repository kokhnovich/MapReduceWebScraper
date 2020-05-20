# $1 - depth
depth="$1"

#time bash ./BuildScripts.sh

while [[ "$depth" -gt 0 ]]; do
  # echo "$depth" &&
  echo "running map..." &&
  time ./mr map ./scraper.py input.txt temp.txt &&

  echo "running reduce..." &&
  time ./mr reduce ./uniq temp.txt output.txt &&

  echo "checking..." &&
  head output.txt &&

  # echo "preparing to the next step..." &&
  wc -l output.txt &&
  rm temp.txt &&
  cat output.txt > input.txt
  ((depth--))
done