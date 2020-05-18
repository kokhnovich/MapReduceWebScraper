#!/usr/bin/python3
from requests import get
from bs4 import BeautifulSoup
import sys


def is_correct(s):
    return len(s) > 8 and (s[:8] == 'https://' or s[:8] == 'http://')


visited = set()
for line in sys.stdin:
    if not is_correct(line): continue
    req = get(line[:-1])
    parser = BeautifulSoup(req.text, 'lxml')

    for link in parser.find_all('a', href=True):
        # @TODO add handlers for links started with '//' and '#'
        ref = link['href']
        if len(ref) < 60 and is_correct(ref):
            print(ref)
