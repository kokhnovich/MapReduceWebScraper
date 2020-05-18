#!/usr/bin/python3
from requests import get
from bs4 import BeautifulSoup
import sys

def is_correct(s):
    return len(s) > 8 and (s[:8] == 'https://' or s[:7] == 'http://')


visited = set()
for line in sys.stdin:
    if not is_correct(line): continue
    try:
        if line[:-1] in visited: continue
        req = get(line[:-1])
        parser = BeautifulSoup(req.text, 'lxml')

        for link in parser.find_all('a', href=True):
            if link in visited: continue
            visited.add(link)
            # @TODO add handlers for links started with '//' and '#'
            ref = link['href']
            if len(ref) < 60 and is_correct(ref):
                print(ref)
    except:
        print("can't get '" + line[:-1] + "'", file=sys.stderr)
