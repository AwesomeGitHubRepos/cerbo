#lang racket
(require macro-debugger/expand)
(require (planet dyoo/while-loop:1:1))


(define desc "
.SYNTAX PROGRAM
ST = .ID .LABEL * '=' EX1 '.,' .,
PROGRAM = '.SYNTAX' .ID $ ST .END .,
.END
")

(define-syntax-rule (each var lst  body ...)
  (for-each (lambda (var) body ...) lst))



(define pin void)
(define (pin! str) (set! pin (open-input-string str)))
(define (pc) (peek-char pin))
(define (rc) (read-char pin))

(define (crange? c lo hi) (and (char? c ) (char>=? c lo) (char<=? c hi)))
(define (letter? c) (or (crange? c #\a #\z) (crange? c #\A #\Z)))
(define (digit? c) (crange? c #\0 #\9))
(define (l-or-d? c) (or (digit? c) (letter? c)))
(define (white? c) (member c '(#\tab #\space #\newline)))
(define (non-white? c) (and (not (white? c)) (not (eof-object? c))))

(define (accum pred)
  (list->string
   (reverse
    (let loop ((res '()))
      (if (pred (pc))
          (loop (cons (rc) res))
          res)))))

;(define (eat-white) (accum white?))
;(define (accum-1 pred) (eat-white) (accum pred))
(define (get-string)
  (rc)
  (define res (accum (lambda (c) (not (char=? c #\')))))
  (rc)
  res)


(define (-yylex)
  ;;(display 1)
  (accum white?) ; eat white-space
  ;;(display 2)
  ;;(display (pc))
  (define res (cond
    [(eof-object? (pc)) (list 'eof (rc))]
    [ (char=? #\' (pc)) (list 'str (get-string))]
    [(digit? (pc)) (list 'number (accum digit?))]
    [(letter? (pc)) (list 'id (accum non-white?))]
    ;;[(non-white? (pc)) (list 'lit (accum non-white?))]  
    [(non-white? (pc))  (list 'lit (string (rc)))]
    [else 'dunno]))
  ;;(display 3)
  res)

(define yytype void)
(define yytext void)
(define (yytype? target) (eq? yytype target))
(define (yytext=? str) (and (string? yytext) (string=? yytext str)))

(define (yylex)
  (define yy (-yylex))
  (set! yytype (car yy))
  (set! yytext (cadr yy)))

(define (out lst)
  (unless (empty? lst)
    (for-each (lambda (x)
                (cond
                  [(eq? x '*) (display yytext)]
                  [else (display x)])
                #t)
              lst)
    (newline)))


(define (expect str)
  ;;(printf "expect called with ~s\n" str)
  (if (yytext=? str)
      (begin
        (yylex)
        #t)
      #f))



(define (number)
  (if (yytype? 'number)
      (begin
        (printf "LD ~a\n" yytext)
        (yylex)
        #t)
      #f))

(define (.yytext txt) (printf "yytext in ~a = ~a\n" txt yytext) #t)

(define (bracket)
 
  (and (expect "(")  (ex1) (expect ")") ))

(define (ex3)
  ;;(.yytext "ex3")
  (or (number) (bracket)))
  
(define (ex2)
  ;;(.yytext "ex2")
  (ex3)
  (while (yytext=? "*")
    (yylex)
    (ex3)
    (displayln "MLT"))
  #t)

(define (ex1)
  ;;(.yytext "ex1a")
  (ex2)
  (while (yytext=? "+")
         ;;(.yytext "ex1b")
         (yylex)
         (ex2)
         (displayln "ADD"))
  #t)


(define (do-it user-data)
  (pin! user-data)
  (yylex)
  (ex1)
  #t)

(do-it "( 22+23)  * ( 24 + 25 )")
;;(do-it "22  + 23")

