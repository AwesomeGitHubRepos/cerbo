import os
import time

import pytext

URL_FILE = os.getenv("DOCS") + "/unclassified/urls.txt"
#print URL_FILE

def retrieve():
    global URL_FILE
    lines = pytext.readlines(URL_FILE)
    for line in lines:
        iid = line[0:3]
        dstamp = line[4:15]
        url = line[16:]
        yield iid, dstamp, url

def add():
    global URL_FILE
    url = raw_input("Enter URL:\n")
    iid, dummy1, dummy2 = retrieve().next()
    nid = "{0:03}".format(int(iid)+1)
    dstamp = time.strftime("%d-%b-%Y")
    line = "{0} {1} {2}".format(nid, dstamp, url)
    print line
    lines = [line] + pytext.readlines(URL_FILE)
    pytext.writelines(URL_FILE, lines)
    #lines = [line].append([retrieve()]

def dump():
    for iid, dstamp, url in retrieve():
        print "iid:*{0}*\ndstamp:*{1}*\nurl:*{2}*\n".format(iid, dstamp, url)

def print_url():
    uid = raw_input("Id:")
    for iid, dstamp, url in retrieve():
        if iid == uid:
            print url
            return

if __name__ == "__main__":
    while True:
        print "(a)dd, (d)ump, (p)rint, e(x)it"
        action = raw_input()
        if action == 'a': add()
        elif action == "d": dump()
        elif action == "p": print_url()
        elif action == "x": exit(0)
        else: print "Error. Didn't understand"
            
