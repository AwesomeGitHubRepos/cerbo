;;(load "vasmc.scm")
;;(require 'posix)
(require-extension byte-blob) ; sudo chicken-install byte-blob
(require 'srfi-4)

(define oport '())

(define (write-bytes list-of-bytes)
  (byte-blob-write oport (list->byte-blob list-of-bytes)))

(define (write-byte b)
  (write-bytes (list b)))

(define (rxy->byte rx ry)
  (+ (arithmetic-shift rx 4) ry))

(define (write-rxy rx ry)
  (write-byte (rxy->byte rx ry)))

(define (write-offset offset)
  (define bytes '())
  (let loop ((n 4))
    (set! bytes (cons (bitwise-and 255 offset) bytes))
    (set! offset (arithmetic-shift offset -8))
    (unless (zero? n) (loop (- n 1))))
  (write-bytes bytes))
    

(define (type-2 instruction-code rx offset ry)
  (write-byte instruction-code)
  (write-rxy rx ry)
  (write-offset offset))


(define (branch id offset)
  (write-byte id)
  (write-offset offset))
  
  

(define (sti rx off ry)
  (type-2 7 rx off ry))

(define (ldi offset rx ry)
  (type-2 8 rx offset ry))

(define (ldr rx ry)
  (write-byte 10)
  (write-rxy rx ry))

(define (bze off) (branch 11 off))

(define (bnz off) (branch 12 off))

(define (bra off) (branch 13 off))

(define (bal rx ry)
  (write-byte 14)
  (write-rxy rx ry))

(begin
  (set! oport (open-output-file "output.dat"))
  (bal 4 5)
  (close-output-port oport)
  #t)

