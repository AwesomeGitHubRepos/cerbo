;;(load "vas-parser.scm")
(require-extension srfi-13)
(require-extension srfi-14)

(require-extension srfi-1)

(require-extension bindings)
;; needing installation:
;;(require-extension srfi-34)
;;(require-extension loop)
;;(require-extension miscmacros)
;;(require-extension matchable)
(require-extension holes)
;;(require-extension anaphora)

(use regex-literals)
(require-extension regex-literals)

;;(import mcutils)
;;(declare (uses mcutils))
(include "mcutils.scm")
(use mcutils)
;;(include "mcutils.scm")


(define-syntax-rule (show-var var)
  (display (quote var))
  (display "=")
  (displayln var))


(define (carf lst) (if (null? lst) '() (car lst)))


;;(parse-file "fact.vas")

(define (white? c) (char-set-contains? char-set:whitespace c))

(define (letter? c) (char-set-contains? char-set:letter c))

(define (white chars) (cons 'WHITE (take-while white? chars)))

(define (word chars) (cons 'WORD (take-while letter? chars)))

(define (comment chars)
  (cons 'COMMENT
	(if (eq? #\\ (carf chars)) chars '())))

(define (cdrf lst) (if (null? lst) #f (cdr lst)))

(define (other chars)
  (list 'OTHER (car chars)))

(define (match-first procs chars)
  (define (loop-0 procs)
    (define matched ((car procs) chars))
    ;;(show-var matched)
    ;;(show-var (length (cdr matched)))
    (if (pair? (cdr matched))
	matched	       
	(loop-0 (cdr procs))))
  (loop-0 procs))



;;(define foo 12)
;;(show-var foo)

(define (lexer matchers str)
  (define (loop-0 matches chars)
    ;;(show-var chars)
    (define match (match-first matchers chars))
    ;;(show-var match)
    (define yytext (cdr match))
    (assert (pair? yytext)) ; you can't just match nothing
    (define remainder (drop chars (length yytext)))
    ;;(show-var remainder)
    ;;(display 0)
    (set! yytext (list->string yytext))
    ;;(display 1)
    (set! match (list (car match) yytext))
    ;;(show-var match)
    (set! matches (cons match matches))
    (if (pair? remainder)
	(loop-0 matches remainder)
	(reverse matches)))
  (loop-0 '() (string->list str)))
    


(define matchers (list white word comment other))

;;(length (word '(#\space)))
(show-var (lexer matchers "foo   (Rval,\\hello world"))

;;(match-first matchers (string->list "foo  bar"))

;;(pair? (take-while white? '(#\r)))

;;(white? #\s)
;;(pair? (cdr (white '(#\r))))
