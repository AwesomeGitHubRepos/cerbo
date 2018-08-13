(module
 mcutils (export define-syntax-rule displayln hello-utils until)
 (import scheme)
 
(define-syntax define-syntax-rule
  (syntax-rules ()
    [(define-syntax-rule (id arg ...) body)
     (define-syntax id
       (syntax-rules ()
	 [(id arg ...) body]))]))

(define-syntax-rule (until break body ...)
  (let* ((continue #t)
	 (break (lambda() (set! continue #f))))
    (let loop ()
      body ...
      (when continue (loop)))))

 (define (displayln text)
   (display text)
   (newline))
 
 (define (hello-utils)
   (displayln "hello utils says hello"))

 ) ; end of module


	
