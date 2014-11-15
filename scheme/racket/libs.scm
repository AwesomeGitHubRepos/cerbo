(require srfi/43)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; formatted print

(define-syntax prin
  (syntax-rules ()
    ((print val) (display val))
    ((prin fmt args ...) (display (format fmt args ...)))))

;;; Examples
;;; (prin 42)
;;; (prin "~% ~s ~%" 1 )
;;; (prin "~% ~s ~% ~s ~%" 1 2)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; defeq - define an equation - should probably use this from now on

(define-syntax defeq 
  (syntax-rules ()
    ((defeq var val) (begin
		       (define var val) 
		       (prin "EQUATION: ~s RESULTS: ~%" (quote var))
		       (prin var)
		       (newline)))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



(define (vector* v1 v2) (vector-map (lambda (i x y) (* x y)) v1 v2))
(define (vector/ v1 v2) (vector-map (lambda (i x y) (/ x y)) v1 v2))
(define (vector+ v1 v2) (vector-map (lambda (i x y) (+ x y)) v1 v2))




(define (vector-do v func state)
  (let ((size (vector-length v)))
    (let kernel ((position 0))
      (if (= position size)
	  state
	  (begin
	    (set! state (func (vector-ref v position) state))
	    (kernel (+ position 1)))))))

(define (vector-sum v)
  (vector-do v + 0))

(define (vector-scale s v) ; scale a vector v by s
  (vector-map (lambda (i x) (* s x)) v))

(define (vector-norm v) ; normalise a vector
  (vector-scale (/ 1 (vector-sum v)) v))