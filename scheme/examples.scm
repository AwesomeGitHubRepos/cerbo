(load "mymacros.scm")


(for x 1 3
     (display x)
     (newline))

(define x 1)
(while (< x 3)
       (display ",")
       (set! x (+ x 1)))

(++ x)


(display (collect bob
	 (bob 1)
	 (for x 10 15
	      (bob x))
	 (bob 42)))
