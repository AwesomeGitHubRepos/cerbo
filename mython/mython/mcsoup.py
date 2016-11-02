import sys

import BeautifulSoup

def stdin_to_soup():
    "read stdin, and convert to soup"
    html = sys.stdin.read()
    soup = BeautifulSoup.BeautifulSoup(html)
    return soup
    
