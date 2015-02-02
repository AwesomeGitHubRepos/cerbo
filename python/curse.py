import curses
from curses import wrapper

def main(stdscr):
    # Clear screen
    stdscr.clear()
    curses.mousemask(-1)
    pad = curses.newpad(100, 100)
    # These loops fill the pad with letters; addch() is
    # explained in the next section
    for y in range(0, 99):
        for x in range(0, 99):
            pad.addch(y,x, ord('a') + (x*x+y*y) % 26)

    pad.refresh( 0,0, 5,5, 20,75)
    # This raises ZeroDivisionError when i == 10.
    #for i in range(0, 11):
    #    v = i-10
    #    stdscr.addstr(i, 0, '10 divided by {} is {}'.format(v, 10/v))

    stdscr.addstr(0, 0, "Current mode: Typing mode",
                  curses.A_REVERSE)
    stdscr.refresh()
    stdscr.getkey()

wrapper(main)
