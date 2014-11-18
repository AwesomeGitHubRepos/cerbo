#lang racket

(require carali/maths)

(require rackunit)

#|
(check-equal? (rank '(13.50  2.93 	 -6.51 	 4.66	 5.80  	 -7.63 	 3.54  	 10.46 	 7.60  	 17.20))
           '(6 3 2 7 4 5 9 8 1 10))
|#

(check-= (hash-ref (exp-fit '(100 110 121)) 'rate) 1.1 0.001)

(check-= (hash-ref (exp-fit '(100 110 121)) 'r2) 1.0 0.001)

(check-equal? (let ((i (integers)))
                (for/list ((j '(10 11 12)))
                  (cons (i) j)))
              '((0 . 10) (1 . 11) (2 . 12)))