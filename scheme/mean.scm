(define (mean lst)
  (/ (apply + lst) (length lst)))

(display (mean '(3.8 -1.4 -0.1 0.7 -0.2)))
(newline)

(exit 0)
