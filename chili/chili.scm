(module
 chili
 (export define-syntax-rule displayln
	 file->lines hello-utils until)
 (import chicken extras scheme data-structures)

;;(declare (unit mcutils))

 (require-extension data-structures)
 (require-extension extras)
;;(import scheme)

(define-syntax define-syntax-rule
  (syntax-rules ()
    [(define-syntax-rule (id arg ...) body ...)
     (define-syntax id
       (syntax-rules ()
	 [(id arg ...) (begin body ...)]))]))

(define-syntax-rule (until break body ...)
  (let* ((continue #t)
	 (break (lambda() (set! continue #f))))
    (let loop ()
      body ...
      (when continue (loop)))))

(define (displayln text)
   (display text)
   (newline))


(define (file->lines filename)	
  (define (loop lines)
    (define line (read-line))
    (if (eof-object? line)
	(reverse lines)
	(loop (cons line lines))))
  (define (loop-0) (loop '()))
  (with-input-from-file filename loop-0))


(define (hello-utils)
   (displayln "hello utils says hello"))

) ; end of module


	
