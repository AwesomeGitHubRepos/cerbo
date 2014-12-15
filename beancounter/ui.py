#!/usr/bin/env python3

import os

import hyh

def run(cmd): os.system(cmd)

class State : pass

def main_menu():
    print("""b backpopulate HYH
p plot
s set symbol
q quit
""")
    #sym = "INDEXFTSE:ASX"
    print("Symbol:", State.sym)
    choice = input()
    if choice == "b":
        run("beancounter backpopulate HYH")
    elif choice == "p": hyh.main()
    elif choice == "s": State.sym = input("Enter new symbol:")
    elif choice == "q": return False
    else:
        print("Unknown choice")

    return True

def main():
    State.sym = "INDEXFTSE:ASX"
    while main_menu(): pass
    print("Quit")

if __name__ == "__main__":
    main()
