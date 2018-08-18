;;(load "vas-parser.scm")
(require-extension srfi-13)
(require-extension srfi-14)

(require-extension srfi-1)


;;(require-extension bindings)
;; needing installation:
;;(require-extension srfi-34)
;;(require-extension loop)
;;(require-extension miscmacros)
;;(require-extension f-operator)
(use shift-reset)
(require-extension holes)
;;(require-extension anaphora)
(require-extension list-utils)
(use regex-literals)
(require-extension regex-literals)

;;(import mcutils)
;;(declare (uses mcutils))
;;(include "mcutils.scm")
(use mcutils)
;;(include "mcutils.scm")


(define-syntax-rule (show-var var)
  (display (quote var))
  (display "=")
  (displayln var))


(define (carf lst) (if (null? lst) '() (car lst)))


;;(parse-file "fact.vas")

(define-syntax-rule (listify! chars)
  (when (string? chars) (set! chars (string->list chars))))

(define (taking cset sym)
  (define (pred c) (char-set-contains? cset c))
  (lambda (chars)
    (listify! chars)
    (define matched (take-while pred chars))
    (if (pair? matched)
	(cons sym matched)
	#f)))

(define word   (taking char-set:letter 'WORD))
(define white (taking char-set:whitespace 'WHITE))
(define number (taking char-set:digit 'NUMBER))

(define (comment chars)
  (listify! chars)
  (if (eq? (carf chars) #\\)
      (cons #f chars)
      #f))

(define (other chars)
  (listify! chars) 
  (if (pair? chars)
      (list 'OTHER (car chars))
      #f))


(define (match-first chars)
  (ormap (lambda (proc) (proc chars)) (list word white number comment other)))

(define (return x)
  (shift k x))

(define (lexify chars)
  (listify! chars)
  (define (loop accum chars)
    (reset
     (when (null? chars) (return accum))
     (define m (match-first chars))
     ;;(show-var m)
     (unless m (return accum))
     (define rest (drop chars (length (cdr m))))
     ;;(show-var rest)
     (if (car m)
	 (loop (cons m accum) rest)
	 (loop accum rest))))
  (reverse (loop '() chars)))

(lexify "HEL WORK,D 23\\ foo")



   


(define (cdrf lst) (if (null? lst) #f (cdr lst)))


(show-var (lexer matchers "  LDI 1000(R1),R2"))

(parse-line "  LDI 1000(R1),R2")
   
