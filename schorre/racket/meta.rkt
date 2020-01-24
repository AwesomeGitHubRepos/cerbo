#lang racket
;(require swindle)
;(require racket/stream)
;(define ns (make-base-namespace))
;(define ns (current-namespace))
;(define (ev x) (eval x (scheme-report-environment 5)))
;(define (ev x) (eval x (null-environment 5)))
;(define (ev x) (eval x))
;(define (ev x) (eval x ns))
;(define (ev x) (apply x '()))


(define desc "
.SYNTAX PROGRAM
ST = .ID .LABEL * '=' EX1 '.,' .,
PROGRAM = '.SYNTAX' .ID $ ST .END .,
.END
")

(define (displays . lst)
  (for-each (lambda (x) (display x) (display " ")) lst)
  (newline))
 
#|
  (if (empty? lst)
      (newline)
      (begin
        (display (car lst))
        (displays (cdr lst)))))
  |#           


(define pin void)
(define (pin! str) (set! pin (open-input-string str)))
(define (pc) (peek-char pin))
(define (rc) (read-char pin))

(define (crange? c lo hi) (and (char? c ) (char>=? c lo) (char<=? c hi)))
(define (letter? c) (or (crange? c #\a #\z) (crange? c #\A #\Z)))
(define (digit? c) (crange? c #\0 #\9))
(define (l-or-d? c) (or (digit? c) (letter? c)))
(define (white? c) (member c '(#\tab #\space #\newline)))
(define (non-white? c) (not (white? c)))

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
  (accum white?) ; eat white-space
  (cond
    [ (char=? #\' (pc)) (list 'str (get-string))]
    [(digit? (pc)) (list 'number (accum digit?))]
    [(letter? (pc)) (list 'id (accum non-white?))]
    [(non-white? (pc)) (list 'lit (accum non-white?))]  
    [else 'dunno]))

(define yytype void)
(define yytext void)

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

(pin! "23 + 24")

(define prog '((.NUMBER "LD " *)
               $ ( ("+") (.NUMBER "LD " * "\nADD") )
               ))
;(define (do-nothing) void)

(define (nested? lst) (and (not (empty? lst)) (not (empty? (car lst)) )))

(define (-run pat outer)
  (displays "\n-run" pat yytype yytext)
  (cond
    [(and (eq? pat '.NUMBER) (eq? yytype 'number))
     (out outer)]
    [(and (eq? yytype 'lit) (string=? pat yytext))
     (out outer)]
    [else (raise "estoy confundido")]))

(define (run lst)
  (unless (empty? lst)
    (cond
      [(eq? (car lst) '$)
       ;(displays (cadr lst))
       (for/list ((el (cadr lst)))
         (run el))
       ;(while (run (cadr lst)) #t)
       ;(raise "TODO process $")
       #t]
      [(list? (car lst))
       (-run (caar lst) (cdar lst))]
      ;[else ; no OUT
       ;(-run (car lst) '())]
      )
    (unless (empty? (cdr lst))
      (yylex)
      (run (cdr lst)))))

(yylex)
(run prog)
