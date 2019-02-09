;(load "mymacros.scm")

;; works with chicken, chez, guile
(include "mymacros.scm")

(for i 1 3
     (display i)
     (newline))

(define i1 1)
(while (< i1 3)
       (display ",")
       (++ i1))


(display (collect bob
	 (bob 1)
	 (for x 10 15
	      (bob x))
	 (bob 42)))



(write (mc-read-lines "scratch.scm"))
