#lang racket ;-*-scheme-*-

(provide formatln print-list)

(require srfi/26) ; for cut
(require srfi/48)

(define (formatln fmt . rst)
  (displayln (apply format (cons fmt rst))))
; Example:
; (formatln "~a ~a" 1 2)
; print 1 2

(define (print-list list)
  (for-each (compose display (cut format "~10,2F " <>)) list)
  (newline))