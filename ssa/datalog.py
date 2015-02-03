from pyDatalog import pyDatalog as pdl


import mython.pytext

def main():
    data = mython.pytext.load("~/.ssa/ssa.dl")
    pdl.load(data)
    print(pdl.ask("etran(A, 'CGS', B, C)"))

if __name__ == "__main__":
    main()
