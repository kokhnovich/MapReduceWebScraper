from requests import get
from bs4 import BeautifulSoup
import sys
assert(len(sys.argv) >= 2)
link = sys.argv[1]
req = get(link)
parser = BeautifulSoup(req.text, 'lxml')
for link in parser.find_all('a', href=True):
    if link['href'][0] != '#':
        print(link['href'])
