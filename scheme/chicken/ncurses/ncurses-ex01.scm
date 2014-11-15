(use ncurses)

(initscr)
(cbreak)
(noecho)
(printw "Press q to exit")
(refresh)


;(define (half a b) (floor (/ (- a b) 2)))
;(define starty (half LINES 3))
;(define startx (half COLS 10))
(define *win* (newwin 6 6 10 10))
(wprintw *win* "Hello world")
(wrefresh *win*)


;;; wait until `q' is pressed
(let loop ()
  (let ((ch (getch)))
    (when (not (equal? ch #\q))
	  (loop))))

;;; clean up and exit
(delwin *win*)
(endwin)
(exit)


