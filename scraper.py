#!/usr/bin/python3
from requests import get
from bs4 import BeautifulSoup
import sys


def is_correct(s):
    return len(s) > 8 and (s[:8] == 'https://' or s[:7] == 'http://')


visited = set()
data = sys.stdin.read().splitlines()
for line in data:
    if not is_correct(line): continue
    try:
        # print("'"+line+"'")
        # print(line.split('\t'))
        address, info = line.split('\t')[:2]
        if info[0] == '0':
            # if address is visited we just rewrite it
            visited.add(address)
            print(address + '\t0')
            continue
        visited.add(address)
        page = get(address, timeout=1)
        parser = BeautifulSoup(page.text, 'lxml')

        for link in parser.find_all('a', href=True):
            if link in visited:
                continue
            visited.add(link)
            # @TODO add handlers for links started with '//' and '#'
            ref = link['href']
            if len(ref) < 117 and is_correct(ref):
                print(ref + '\t1')
        print(address + '\t0')
    except:
        print(sys.exc_info()[0], file=sys.stderr)
        print("can't get '" + line + "'", file=sys.stderr)
