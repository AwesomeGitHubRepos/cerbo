;;(load "shlex.scm")
(require-extension big-chicken)
(require-extension fmt)
(require-extension chili)

(shlex-line "how now brown cow")
(define variables (make-hash-table))
(define (get-var varname)
  (hash-table-ref/default variables varname 0))
(define (set-var varname value)
  (hash-table-set! variables varname value))




(define (shrec-l path cmd field-names thunk)  
  (do-list line (file->lines path)
	   (let ((field-values (shlex-line line)))
	     ;;(print field-values)
	     (when (and (pair? field-values) (string= cmd (car field-values)))
		   ;;(print 0)
		   (do-list k-v (zip (map symbol->string field-names)
				     (cdr field-values))
			    (apply set-var k-v))
		   (thunk)))))

(define-syntax-rule (shrec path cmd field-names body ...)
  (shrec-l path cmd 'field-names (lambda () body ...)))

(define path "/home/mcarter/repos/redact/docs/accts2018/accts2018v2.txt")

(shrec path "etran-3" (dstamp acc sym qty amount desc)
       (fmt #t (get-var "dstamp") " " (- (+ 0.1 0.2) 0.3) " " (get-var "qty") nl)
       #t)
