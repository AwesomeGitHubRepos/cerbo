import os
import time

import pyfiglet

#secs = 100

def loop(secs):
    while secs > 0:
        os.system("clear")
        s = secs % 60
        mins = secs // 60
        out = '{:02}:{:02}'.format(mins, s)
        out1 = ""
        for c in out: out1 += c + "  "
        pyfiglet.print_figlet(out1)
        time.sleep(1)
        secs -= 1


if __name__ == "__main__":
    loop(1165)
    
