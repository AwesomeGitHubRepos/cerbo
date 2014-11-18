#lang racket

(require  carali
          ;carali/prog
          rackunit)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; test catch-errors


(check-equal? (catch-errors "Oops" (/ 1 0)) 
              "Oops" 
              "Generate an error")

(check-eq? (catch-errors "Oops" (/ 6 3)) 2 "Simple check on catch-errors")

(check-eq? (catch-errors "Oops" (/ 6 3) (/ 16 2)) 8 "Check multiple forms in thunk")


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; test defines

#|
(check-equal? (progn ;can't use begin
                (defines a b)
                (set! a 42)
                (list a b))
              (list 42 'undefined)
              "Basic defines functionality")
  |#                 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(check-eq? (let ((a 1))
             (inc a)
             a)
           2
           "inc default adds 1")

(check-eq? (let ((a 10))
             (inc a 11)
             a)
           21
           "inc default adds 11 to 10")