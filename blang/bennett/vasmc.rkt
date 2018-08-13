#lang racket

(require data/queue)
;;(require rnrs/bytevectors-6)
(require binaryio)

;;(define registers (make-vector 16))

(define byte-code (make-queue))
(define (b+ b) (enqueue! byte-code b))

;;(define byte-code-str (open-output-bytes))
;;(define (write-byte b) (write b byte-code-str))

;;(define add-byte write-byte)

(define (add-byte b)
  (displayln "add-byte called")
  (enqueue! byte-code (bytes b)))

(define (add-bytes bs)
  (for-each b+ bs))

(define (vas-halt)
  (add-byte 0))

(define (vas-nop)
  (add-byte 1))

(define (vas-trap)
  (add-byte 2))

(define (pack a b)
  (+ (* 16 a) b))


(define-syntax-rule (define-math-op id num)
  (define (id a b)
    (add-bytes num (pack a b))))

(define-math-op vas-add 3)
(define-math-op vas-sub 4)
(define-math-op vas-mul 5)
(define-math-op vas-div 6)

(define (vas-sti rx offset ry)
  (define (conv v) (add-byte (bytes-ref (integer->bytes v 1 #t) 0)))
  (conv 7)
  (add-byte rx)
  (add-bytes (bytes->list (integer->bytes offset 4 #t)))
  (add-byte ry))

  


;;(define (vas-mov v reg)
;;  (vector-set! registers reg v))

#|
  (define r15 (vector-ref registers 15))
  (define c (bitwise-bit-field r15 0 8))
  (define c1 (integer->char c))
  (display c1))
|#

#|
(define (vas-interpret code)
  (define pc 0)
  (define (pc++) (set! pc (+ 1 pc)))
  (define halt #f)
  (let loop ()
    (define instruction (list-ref code pc))
    (case (car instruction)
      [(vas-halt) (add-byte 0) (set! halt #t)]
      [(vas-mov)
       (apply vas-mov (rest instruction))
       (pc++)]
      [(vas-trap)
       (apply vas-trap (rest instruction))
       (pc++)]
      [else
       (displayln (format "unknown instruction: ~s. Aborting." instruction))
       (set! halt #t)])
    ;;(when (> pc 1000)
    ;;  (displayln "Seems suspiciously like stack overflow. Aborting.")
    ;;  (set! halt #t))
    (unless halt (loop))))
|#

;;(define (vas-interpret code)
;;  (for-each (lambda (x) (apply (car x) (rest x))) code))

  

(define (code)
  (begin
    (vas-sti 0 000 0)
    ;;(vas-halt)
    ;;(vas-div 0 10)
    ;;(vas-nop)
    ;;(vas-mov 88 15) ;; 88 is an X
    ;;(vas-trap)
    ;;(vas-halt)
    #t))

(code)

;;code
;;(vas-interpret code)

#|
(display (queue->list byte-code))
(with-output-to-file "output.vam"
  (lambda ()
    (for-each (lambda (x) (display x))
              (queue->list byte-code)))
  #:exists 'replace)

(integer->bytes -26 4 #t)
|#

#|
(with-output-to-file "output.vam"
  (lambda () (display (get-output-bytes byte-code-str)))
  #:exists 'replace)
|#


(define out (open-output-file "output.vam" #:mode 'binary #:exists 'replace))
(for-each
 (lambda (b)
   (displayln b out)
   (displayln b))
 ;;(bytes->list (get-output-bytes byte-code-str)))
 (queue->list byte-code))
;;(write-bytes (get-output-bytes byte-code-str) out)
(close-output-port out)

(displayln "Some bytes")
(for-each
 (lambda (b) (displayln b))
 ;;(bytes->list (get-output-bytes byte-code-str)))
 (queue->list byte-code))
(queue->list byte-code)
