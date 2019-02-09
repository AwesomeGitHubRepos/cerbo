(use-modules (ice-9 rdelim))

;; simplify the definition of syntaxes
;(def-syntax define-syntax-rule
;  (syntax-rules ()
;    [(define-syntax-rule (id arg ...) body)
;     (define-syntax id
;       (syntax-rules ()
;	 [(id arg ...) body]))]))


;(def-syntax (while test) . body

(define-syntax while
  (syntax-rules ()
    ((_ condition . body)
     (let loop ()
       (cond (condition
	      (begin . body)
	      (loop)))))))

(define-syntax ++
  (syntax-rules ()
    ((_ var)
     (set! var (+ 1 var)))
    ((_ var by)
     (set! var (+ by var)))))

(define-syntax for
  (syntax-rules ()
    ((_ var lo hi . body)
     (let ((var lo))
       (while (<= var hi)
	      (begin . body)
	      (++ var))))))


(define-syntax collect
  (syntax-rules ()
    ((_ collector . body)
     (let* ((acc '())
	    (collector (lambda (el) (set! acc (cons el acc)))))
       (begin . body)
       (reverse acc)))))



(define (mc-read-lines filename)
  (define (loop lines)
    (define line (read-line))
    (if (eof-object? line)
	(reverse lines)
	(loop (cons line lines))))
  (define (loop-0) (loop '()))
  (with-input-from-file filename loop-0))
