;;(load "vasmc.scm")
;;(require 'posix)

;; must be installed:
(require-extension byte-blob) ; sudo chicken-install byte-blob
(use bindings)

(require 'srfi-1)
(require 'srfi-4)
(require 'srfi-13)

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
    


(define opcode-table
  ;; OPCODE ID Rn? OFF?
  '(("HALT"  0 #f  #f)
    ("NOP"   1 #f  #f)
    ("TRAP"  2 #f  #f)
    ("ADD"   3 #t  #f)
    ("SUB"   4 #t  #f)
    ("MUL"   5 #t  #f)
    ("DIV"   6 #t  #f)
    ("STI"   7 #t  #t)
    ("LDI"   8 #t  #t)
    ("LDA"   9 #t  #t)
    ("LDR"  10 #t  #f)
    ("BZE"  11 #f  #t)
    ("BNZ"  12 #f  #t)
    ("BRA"  13 #f  #t)
    ("BAL"  14 #t  #f)))

(define (write-op opcode rx ry offset)
  (define rec (find (lambda (x) (string= (car x) opcode)) opcode-table))
  (bind (opnum rn? off?) (cdr rec)
	(write-byte opnum)
	(when rn? (write-rxy rx ry))
	(when off? (write-offset offset))))

(begin
  (set! oport (open-output-file "output.dat"))
  (write-op "BAL" 4 5 0)
  (close-output-port oport)
  #t)

