(import
  (rnrs)
  ;(srfi :1)
  (srfi :26)
  (ironscheme clr)
)

(define-syntax prin
  (syntax-rules ()
    ((print val) (display val))
    ((prin fmt args ...) (display (format fmt args ...)))))

(define nil '())
(define-syntax vars
  (syntax-rules () ; notice how it's a recursive definition
    ((vars) nil)
    ((vars v) (define v nil))
    ((vars v1  v2 ...) (begin
			 (vars v1)
			 (vars v2 ...)))))

(define (foo)
  (vars a b)
  (set! a 41)
  (set! b 1)
  (prin (+ a b)))

(foo)
(prin "\n~s\n\n" 43)
