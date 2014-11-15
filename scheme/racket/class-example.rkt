#lang racket

;;;; A simple example of defining and using classes in Racket
  
(define point%
  (class object%
    (super-new)
    (init-field (x 0) (y 0))
    (define/public (radius)
      (sqrt (+ (* x x) (* y y))))))

(define p (make-object point%))
(set-field! x p 3)
(set-field! y p 4)
(send p radius)
(get-field x p)