#lang racket

(provide λtk)
(define (λtk test key ) (λ(a b) (test (key a) (key b))))
; Eg ((λtk = car ) '(1 2) '(1 3)) ; #t