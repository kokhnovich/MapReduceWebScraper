from requests import get
from bs4 import BeautifulSoup
import sys
assert(len(sys.argv) == 3)
input_file = sys.argv[1]
output_file = sys.argv[2]
visited = set()
with open(output_file, 'w') as outp:
  with open(input_file, 'r') as inp:
    for line in inp:
      # print("'"+line[:-1]+"'")
      req = get(line[:-1])
      parser = BeautifulSoup(req.text, 'lxml')
      
      for link in parser.find_all('a', href=True):
        ref = link['href'] 
        if ref[0] == '#' or len(ref) <= 2: continue
        if ref[:2] == '//':
          outp.write("https:" + link['href']+'\n')
        else:
          outp.write(ref+'\n')
