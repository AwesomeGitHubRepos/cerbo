;;;(use-modules (ice-9 rdelim))
;;;(use-modules (srfi srfi-1))

(define-syntax collecting
  (syntax-rules ()
		((_ collector body ...)
		 (let* ((acc '())
		       (collector (lambda (x) (set! acc (cons x acc)))))
		   body ... (reverse acc)))))

(collecting yield
	    (yield 12)
	    (display "you could use something other than 'yield'")
	    (newline)
	    (yield (+ 1 12))
	    (yield "easy peasy")
	    (yield "lemon squeezy"))
;;; => (12 13 "easy peasy" "lemon squeezy")


;;(call-with-input-file 
;;  "gums-init.scm"
;;  (lambda (p) 
;;	  (collecting collect 
;;	    (do ((line (read-line p) (read-line p)))
;;	      ((not (eof-object? line))
;;	       (display line)
;;	       (newline)
;;	      (collect line))))))

