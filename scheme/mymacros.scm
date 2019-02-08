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
