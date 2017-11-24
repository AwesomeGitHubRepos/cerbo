;;;; gums: guile module system
;;;; 2017-11-24 mcarter created

;;(use-modules (ice-9 textual-ports))
(use-modules (ice-9 rdelim))
(use-modules (srfi srfi-1))

(define-syntax gums-strcat 
  (syntax-rules ()
		((_ arg1 ...)
		 (string-concatenate (list arg1 ...)))))

(define (gums-file->list path)
  (define (read-lines p) 
    (let loop ((acc '()))
      (let ((line (read-line p)))
	(if (eof-object? line)
	  acc
	  (loop (cons line acc))))))
  (reverse (call-with-input-file path read-lines)))

(define (gums-for-each func lst)
    (let loop ((rest lst))
      (unless (null? rest)
	(func (car rest))
	(loop (cdr rest)))))


(define (gums-lines->file alist path)
  (let ((p (open-output-file path)))
    (gums-for-each (lambda (line) (display line p) (newline p)) alist)
    (close-output-port p)))



(define gums-dir (gums-strcat (getenv "HOME") "/gums"))
(define gums-init-file (gums-strcat gums-dir "/gums-init.scm"))

(define (gums-displayln obj)
  (display obj)
  (newline))


(define (read-guile-rc-file path)
  (let* ((hdr ";;; gums-begin")
	 (appendix (gums-strcat hdr "\n(define gums-use #t)\n(load \""
				gums-init-file "\")\n"))
	 (inlines (gums-file->list path))
	 (hdr? (find (lambda (x) (equal? x hdr)) inlines)))
    (unless hdr?
      (gums-lines->file (append inlines (list appendix)) path))))

(define (update-guile-rc-file)
  (let ((guile-rc-file (gums-strcat (getenv "HOME") "/.guile")))
    (read-guile-rc-file guile-rc-file)
    ;;;(call-with-input-file guile-rc-file read-guile-rc-file)
    #t))

(define (gums-install)
  (display "Installing gums\n")
  (unless (access? gums-dir X_OK)
    (mkdir gums-dir))
  (copy-file "gums-init.scm" gums-init-file)
  (update-guile-rc-file)
  #t)

(unless (defined? 'gums-use)
  (gums-install))
