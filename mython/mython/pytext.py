# ASCII character table:
# http://www.bbdsoft.com/ascii.html

import os.path

def load(filename):
    "Read a file as a string"
    filename = os.path.expanduser(filename)
    try:
        with open(filename,'r') as fp:
            text = fp.read()
    except IOError: text = ""
    return text

def readlines(filename):
    filename = os.path.expanduser(filename)
    with open(filename) as f:
        alist = [line.rstrip() for line in f]
    #lines = open(filename).read().splitlines()
    return alist
    


def save(filename, text):
    'Save text as file'
    filename = os.path.expanduser(filename)
    fp=open(filename, 'w')
    fp.write(text)
    fp.close()

def writelines(filename, list_of_strings):
    text = "\n".join(list_of_strings)
    save(filename, text)

def append_file(filename, text):
    old_text = load(filename)
    save(filename, old_text + text)

def cm2cn(filename):
    "Convert ^M to \n"
    text = load(filename)
    new = ""
    for i in range(0, len(text)):
        if ord(text[i]) == 13: 
            new += '\n'
        else: 
            new += text[i]
    save(filename, new)

def tounix(text):
    "Convert text to unix file endings"
    return text.replace("\r\n", "\n")

def file2unix(filename):
    "Load, convert to UNIX file endings, and save a file"
    text = load(filename)
    save(filename, tounix(text))
    
