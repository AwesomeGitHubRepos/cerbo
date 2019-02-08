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
     (set! var (1+ var)))
    ((_ var by)
     (set! var (+ by var)))))

(define-syntax for
  (syntax-rules ()
    ((_ var lo hi . body)
     (begin
       (define var lo)
       (while (<= var hi)
	      (begin . body)
	      (set! var (1+ var)))))))
