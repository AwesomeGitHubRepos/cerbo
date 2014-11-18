#lang racket

(provide catch-errors define-simple-syntax defines desc    
         id make-sort-pred todo vars)


(define-syntax catch-errors
  ; (catch-errors error-value thunk) - execute thunk, returning it's last value; return error-value if there is an exception
  ; EXAMPLES:
  ; (catch-errors "Oops" (/ 1 0)) ; "Oops"
  ; (catch-errors "Oops" (/ 6 3)) ; 2
  ; (catch-errors "Oops" (/ 6 3) (/ 16 2)) ; 8
  (syntax-rules ()
    ((_ error-value body ...)
     (with-handlers ([exn:fail? (lambda (exn) error-value)])
       body ...))))

; Syntax for defining macros in a simple style similar to function definiton,
;  when there is a single pattern for the argument list and there are no keywords.
;
; (define-simple-syntax (name arg ...) body ...)
;
; example:
;  (define-simple-syntax (+= variable value)
;  (set! variable (+ variable value)))
;(define v 2)
;(+= v 7)
;v ; => 9
; http://www.cs.toronto.edu/~gfb/scheme/simple-macros.html
(define-syntax define-simple-syntax
  (syntax-rules ()
    ((_ (name arg ...) body ...)
     (define-syntax name (syntax-rules () ((name arg ...) (begin body ...)))))))


(define-syntax defines
  ; DEFINE multiple variables, initialising them to 'undefined
  ; EXAMPLE
  ; (progn ;can't use begin
  ;   (defines a b)
  ;   (set! a 42)
  ;   (list a b)) 
  ; => '(42 'undefined)
  (syntax-rules ()
    ((defines a) (define a 'undefined))
    ((defines a b ...) (begin (defines a) (defines b ...)))))






(define-syntax vars
  ; define a list of variables inititalised to null. 
  ; Example
  ; (vars (a b c)
  ;    (print a) ; => prints '() because you haven't set it yet
  ;    (set! b 42)
  ;    (print b)) ; => prints 42
  ; See also the function rank for a more complex example
  (syntax-rules () ; notice how it's a recursive definition
    ;((vars) nil)
    ((vars (v) body ...) 
     (let ((v null))
       body ...))
    
    ((vars (v1  v2 ...) body ...) 
     (let ((v1 null))
       (vars (v2 ...) body ...)))))

(define (make-sort-pred lst)
  (lambda (v1 v2)
    (let/ec return
      (for ((less-than? lst))
        (when (less-than? v1 v2) (return #t))
        (when (less-than? v2 v1) (return #f)))
      #f)))






(define-simple-syntax (todo body ...) "todo")
; Example: (todo (something) (unimplemented)) will just return "todo"

(define (id x) x) ;identity function




(define-simple-syntax (desc form)
  (displayln 'form)
  form)
; eg (desc (+1 2)) print out
; (+1 2)
; 3
; It's useful for tracing and debugging purposes; and for echoing commands to output


(provide nontrivial?)
(define (nontrivial? obj)
  ;determines if an object is non-trivial
  (if (string? obj)
      (< 0  (string-length obj))
      (if (list? obj)
          (empty? obj)
          (raise "nontrivial? unhandled object type" #t))))

(provide whence)
(define-simple-syntax (whence obj body ...)
  (when (nontrivial? obj)
    body ...))

; Example (whence "hello" (print "do something")

