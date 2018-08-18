;;(load "ezvas.scm")
(use srfi-69)

;;(require-extension mcutils)
(include "mcutils.scm")
(use mcutils)

(define-syntax-rule (inc var)
  (set! var (+ 1 var)))

(define ptr 0)



(define (get-inputs)
  (define inp (open-input-file "test.scm"))
  (define (loop accum)
    (define x (read inp))
    ;;(print (list? x))
    (if (eof-object? x)
	  (reverse accum)
	  (loop (cons x accum))))
  (define result (loop '()))
  (close-input-port inp)
  result)

(define registers (make-hash-table))

(define (num->reg num reg)
  (hash-table-set! registers reg num))

(define (reg->num reg)
  ;;(display "HO")
  (if (hash-table-exists? registers reg)
      (hash-table-ref registers reg)
      0))

(define (trap)
  (display (integer->char (reg->num 'A))))
  

(define (halt)
  #t)

(define c1 (car (get-inputs)))

(define (run)
  (set! ptr 0)
  (define opcodes (get-inputs))
  (define (eval-opcode  oc)
    (apply (eval (car oc) (interaction-environment)) (cdr oc)))
  (let loop ()
    (define opcode (list-ref opcodes ptr))
    (unless (eq? (car opcode) 'halt)
	    ;;(display (car opcode))
	    (eval-opcode opcode)
	    (inc ptr)
	    (loop))))

(run)
    
