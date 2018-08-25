(module
 chili
 (export define-syntax-rule defq displayln do-list
	 file->lines hello-utils shlex-line )
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

(define-syntax-rule (defq var queuer)
  (begin
    (define var (make-queue))
    (define (queuer lst) (queue-add! var lst))))


(define (build-shlex-token sp members)
  (define token (open-output-string))
  (let loop ()
    (define c (read-char sp))
    (unless (eof-object? c)
	    (unless (member c members)
		    (display c token)
		    (loop))))
  (define res (get-output-string token))
  (close-output-port token)
  res)

(define (shlex-line line)
  (define sp (open-input-string line))
  (defq fields f+)
  (define (enq members) (f+ (build-shlex-token sp members)))
  (let loop ()
	 (define c (peek-char sp))
	 (unless (eof-object? c)
		 (case c
		   [(#\space #\tab) (read-char sp) (loop)]
		   [(#\#) #t]
		   [(#\") (read-char sp) (enq '(#\")) (loop)] ; string
		   [else (enq '(#\space #\tab #\#)) (loop)]))) ; word
  (close-input-port sp)
  (queue->list fields))

(define (hello-utils)
   (displayln "hello utils says hello"))


(define-for-syntax (do-list-lambda proc lst)
  (let loop ((lst1 lst))
    (when (pair? lst1)
	  (proc (car lst1))
	  (loop (cdr lst1)))))

(define-syntax-rule (do-list var lst body ...)
  (do-list-lambda
   (lambda (x)
     (let ((var x))
       body ...))
   lst))
#|
 Example usage:
(do-list i '(10 11)
	 (print i)
	 (print i))
|#



) ; end of module


	
