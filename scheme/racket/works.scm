(require srfi/26)


(define (always-true exn) #t)
(define (always-false exn) #f)

(define-syntax works?
  (syntax-rules ()
    ((works <test>)
     (with-handlers ((always-true always-false))
<test>
#t))))

(define (test)
  (define foo 20)
  (works? foo) ;;; => true
  (works? bar) ;;; => false (bar is undefined)
  (works? (/ 1 1)) ;;; => true
  (works? (/ 1 0)) ;;; => false
  #t)
