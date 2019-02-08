(load "mymacros.scm")


(for x 1 3
     (display x)
     (newline))

(define x 1)
(while (< x 3)
       (display ",")
       (set! x (+ x 1)))

(++ x)
