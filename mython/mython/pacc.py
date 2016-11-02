#!/usr/bin/env python3

import os
import subprocess

# https://pypi.python.org/pypi/SimpleMenu/
from SimpleMenu import Menu # pip3 install simplemenu

from termcolor import colored


def run_hssa(header, terminator) :
    hssa = subprocess.check_output("hssa")
    hssa_str = hssa.decode("ascii")
    hssa_lines = hssa_str.split("\n")

    hit = False
    region = []
    #result = ""
    for line in hssa_lines:
        if line == header: hit = True
        if hit: region.append(line)
        if line == terminator: hit = False

    coloured_region = ""
    for i, line in enumerate(region):
        if i % 3 == 0: line = colored(line, "green", "on_grey")
        coloured_region += line + "\n"
 
    result_bytes = coloured_region.encode("ascii")
    with subprocess.Popen(['less','-r'], stdin = subprocess.PIPE) as less:
        less.stdin.write(result_bytes)

def edit_menu():
    while True:
        menu = Menu("back", "coms", "etrans", "ntrans", 
                    title = "Edit input files")
        choice = menu.show()
        if choice == "back": return
        editor = os.getenv("EDITOR")
        fname = os.getenv("REDACT") + "/docs/accts2014/data/" + choice + ".txt"
        #fname = "/home/mcarter/redact/docs/accts2014/data/" + choice + ".txt"
        os.system(editor + " " + fname)
        #os.wait()
    #Menu(fname)
    #print(choice)
    #quit()

def view_menu():
    while True:
        menu = Menu("back", "FINANCIALS", "EPICS", "DPSS", "ETB", "ETRANS", 
                    "PORTFOLIOS", "RETURNS", title = "View accounts")
        choice = menu.show()
        if choice == "back": 
            return
        #elif choice == "EPICS": 
        #    choice = "EPICS: ALL"
        else: 
            choice += ":"
        run_hssa(choice, ".")

def accs_menu():
    while True:
        menu = Menu("back", "hal", "hl", "int", "rbs", title = "Accounts")
        choice = menu.show()
        if choice == "back": 
            return
        else:
            run_hssa("Acc: " + choice, ";")

def main_menu():
    while True:
        menu = Menu("quit", "view", "snap", "accs", "edit",
                    title = "Main menu")
        choice = menu.show()

        if choice == "quit" : return
        elif choice == "view": view_menu()
        elif choice == "snap": os.system('snap')
        elif choice == "accs": accs_menu()
        elif choice == "edit": edit_menu()
        else : raise KeyError("Not all main menu items have benn covered")

def main():
    main_menu()
    #edit_inputs()

if __name__ == "__main__":
    main()

#print(hssa)
