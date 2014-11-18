#lang racket

;; lisp translation
(provide find mapcar setf string-equal terpri)

(require carali/misc)



(define (find item alist #:test (test equal?) #:key (key id))
  (let loop ((rest alist))
    (if (null? rest)
        null
        (if (test item (key (car rest)))
            (car rest)
            (loop (cdr rest))))))
;;(find 3 '((1 2) (3 4)) #:key car) => (3 4)

(define mapcar map)

(define-simple-syntax (setf var value) (set! var value))

(define string-equal string=?)

(define terpri newline)




(provide progn)
(define-syntax-rule (progn body ...)
  (let ()
    body ...))
#| 
 ;;example usage
(define foo 
  (progn 
   (define bar 41)
   (+ bar 1)))
foo ; => 42

;;(load "progn.rkt")
|#

