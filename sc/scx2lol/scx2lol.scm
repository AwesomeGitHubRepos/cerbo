(require-extension srfi-1)
;;(require-extension foof-loop)
(require-extension srfi-34) ;exceptions
;;(require-extension srfi-25) ; multi-dimensional arrays
(require-extension srfi-42) ; eager comprehensions
;;(require-extension srfi-47)
(require-extension srfi-63) ; arrays

(require-extension mccsl)

;(load "~/repos/tacc/mccsl/mccsl.scm")
;(import mccsl)

;;(define foo '())
;;(addend! foo 12)


(define (simplify-sc-entry entry)
  (define pos (second entry))
  (define col (second pos))
  (define row (third pos))
  (define val (third entry))
  (list row (- col 1) val))


(define (collect-unthrown func alist)
  (define result '())
  (for-each (lambda (el)
              (with-exception-handler (lambda (x) #f)
                                      (lambda () (addend! result (func el)))))
            alist)
  result)
  
(define (sc-lol p)
  ; make a list-of-lists from an input port
  (define sexpr (read p))
  (define cells (collect-unthrown simplify-sc-entry sexpr))
  (set! cells (filter (lambda (x) (atom? (third x))) cells))
  (define nrows (+ 1 (apply max (map first cells))))
  (define ncols (+ 1 (apply max (map second cells))))
  ;(define ar (make-array (shape 0 nrows 0 ncols) '()))
  (define ar (make-array '#(()) nrows ncols))
  (for-each (lambda (x) 
              ;;(print x)
              (array-set! ar (third x) (first x) (second x) ))
            cells)

;  (define lol (list-ec (: r 0 nrows)
 ;                      (list-ec (: c 0 ncols)
  ;                              (array-ref ar r c))))
  (define lol (array->list ar))
  lol)
                                              
;;(define result (call-with-input-file "~/repos/tacc/sc/scpt/examples/output" sc-lol))

;;(current-input-port)


(define debug #f)

(if debug
    (begin
      (write (call-with-input-file   
                 "~/repos/tacc/sc/scpt/examples/output" sc-lol)))
    (begin
      (write (sc-lol (current-input-port)))))



