#!/usr/bin/env python

# requires pytz
# http://pytz.sourceforge.net/
# easy_install --upgrade pytz

# Actually, the link:
# http://tech.slashdot.org/article.pl?sid=09/02/08/2043206
# suggests a UNIXy solution is available:
# $ TZ="right/US/Eastern" date; TZ="US/Eastern" date
# Sun Feb 8 17:52:42 EST 2009
# Sun Feb 8 17:53:06 EST 2009


#import datetime
from datetime import datetime, timedelta
from multiprocessing import Process
from pytz import timezone # sudo apt-get install python-tz
import pytz
import os
import sys
from time import sleep

from mython.compat import princ

try:
        import msvcrt # will throw an exception if not on windows
        def clear(): os.system('cls')
        def is_q_hit():
                while msvcrt.kbhit():
                        if msvcrt.getch() == "q": return True
                return False
        
        def cleanup():
                return
        def output(text):
                princ(text)
except ImportError:
        import curses
        screen = curses.initscr()
        screen.nodelay(True)
        curses.noecho()
        curses.curs_set(0) # invisible
        screen.clear()
        line = 0
        
        def clear():
                global line, screen
                #screen.clear() #os.system('clear')
                line = 0

        def cleanup():
                curses.endwin()

        def is_q_hit():
                global screen
                while True:
                        ch = screen.getch()
                        if ch == -1: return False
                        if ch == ord('q'): return True
                        
        def output(text):
                global line, screen
                screen.addstr(line, 0, text)
                #curses.nl()
                screen.touchwin()
                screen.refresh()
                line += 1


def killer():
        if is_q_hit():
                cleanup()
                quit()

def print_time(zone, location = None):
	n = datetime.now(pytz.timezone(zone))
	as_string = n.strftime("%a %H:%M" )
	if location == None: location = zone
	txt = "%30s %s" % (location, as_string)
	output(txt)


def print_info():
        clear()
        print_time("US/Mountain", "Salt Lake City")
        print_time("US/Eastern", "Miami - Charles, Mike")
        print_time("US/Eastern", "New York - Calvin Poorman")
        print_time("Europe/London")
        print_time("Asia/Kuala_Lumpur", "Kuala Lumpur")
        output(str(datetime.now()))

        
def main():
        while True:
                print_info()
                for i in range(0,5):
                        sleep(1)
                        killer()


if __name__ == "__main__":
        main()
