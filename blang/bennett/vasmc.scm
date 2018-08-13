;;(require 'posix)
(require-extension byte-blob) ; sudo chicken-install byte-blob
(require 'srfi-4)


(define oport (open-output-file "output.dat"))

(define (sti rx off ry)
  (define byte1 (+ (arithmetic-shift rx 4) ry))
  (define val off)
  (define b1 (bitwise-and 255 val))
  (set! val (arithmetic-shift val -8))
  (define b2 (bitwise-and 255 val))
  (set! val (arithmetic-shift val -8))
  (define b3 (bitwise-and 255 val))
  (set! val (arithmetic-shift val -8))
  (define b4 (bitwise-and 255 val))
  (byte-blob-write oport (list->byte-blob (list 7 byte1 b4 b3 b2 b1)))
  #t)

(sti 12 20000 4)

(close-output-port oport)

