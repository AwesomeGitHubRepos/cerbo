#lang racket

;(require mzlib/string)

(require carali/lists)
 
;download a share price
;usage racket -l carali/money/yfetch FUNC EPIC
;FUNC is one of oeic or yafi
;EPIC is the epic


(require carali/epics)
(require carali/formatting)
(require carali/lists)

(define ccla (current-command-line-arguments))
(define args (vector->list (vector-map string-upcase ccla)))

(define command (string->symbol (first args)))


(define (download)
  ;(print "executing downloader")
  (define epic (cadr args))
  (define fetcher #f)
  (if (string=? "OEIC" (car args))
      (set! fetcher oeic)
      (set! fetcher yafi))
  (print-yafi-line fetcher epic)
  (void))


(define command-dispatcher 
  (encons (list 'OEIC download
                'YAFI download
                )))


(let loop ((pairs command-dispatcher))
  ; TODO handle case when I couldn't find a command
  (define pair (first pairs))
  (define cmd (car pair))
  (define func (cdr pair))
  ;(formatln "~s ~s" command cmd)
  (if (eqv? cmd command)
      (func)
      (unless (empty? (cdr pairs))
        (loop (cdr pairs)))))
      
  
