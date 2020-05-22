#!/usr/bin/python3
from requests import get
from bs4 import BeautifulSoup
import sys
import re

link_regex = re.compile(
    r'^(?:http|ftp)s?://'  # http:// or https://
    r'(?:(?:[A-Z0-9](?:[A-Z0-9-]{0,61}[A-Z0-9])?\.)+(?:[A-Z]{2,6}\.?|[A-Z0-9-]{2,}\.?)|'  # domain...
    r'localhost|'  # localhost...
    r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})'  # ...or ip
    r'(?::\d+)?'  # optional port
    r'(?:/?|[/?]\S+)$', re.IGNORECASE)


def is_correct(link_to_check):
    return re.match(link_regex, link_to_check)


visited = set()
data = sys.stdin.read().splitlines()
for line in data:
    try:
        address, info = line.split('\t')[:2]
        try:
            if not is_correct(address):
                continue
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
                if is_correct(ref):
                    print(ref + '\t1')
            print(address + '\t0')
        except:
            print(sys.exc_info()[0], file=sys.stderr)
            print("can't get '" + line + "'", file=sys.stderr)
    except:
        print('can\'t split ' + line, file=sys.stderr)
