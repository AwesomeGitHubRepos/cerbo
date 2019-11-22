(define (rl) (load "fig3.scm"))

(define labels (make-hash-table))


(define make-stack
  (lambda ()
    (let ([ls '()])
      (lambda (msg . args)
	(cond
	  [(eqv? msg 'ls) ls]
	  [(eqv? msg 'empty?) (null? ls)]
	  [(eqv? msg 'push!) (set! ls (cons (car args) ls))]
	  [(eqv? msg 'top) (car ls)]
	  [(eqv? msg 'pop!) (let ((top (car ls))) (set! ls (cdr ls)) top)]
	  [else "oops"])))))


(define stk (make-stack))
(define (push x) (stk 'push! x))
(define (pop) (stk 'pop!))

(define heap (make-vector 100))



(define hptr 0)

(define-syntax inc
  (syntax-rules ()
    ((_ x by)
     (set! x (+ x by)))
    ((_ x)
     (inc x 1))))

(define-syntax B
  (syntax-rules ()
    ((_ x)
     (display (string-append "B TODO: " (symbol->string (quote x)))))))

(define (BLK n)
  (inc hptr n))


(define (LDL x) (push x))


(define (ADD) (push (+ (pop) (pop))))

(B A01)
(BLK 1)
(LDL 0)
(push 100)
(push 200)
(ADD)
(display (stk 'ls))
