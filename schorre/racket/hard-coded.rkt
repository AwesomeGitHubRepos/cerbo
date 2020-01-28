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
(define (dotty? c)
  (and (non-white? c)
       (not (char=? #\( c))))

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
    [(char=? #\. (pc)) (list 'special (accum dotty?))]
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


(define ** '**)
(define mtext #f) ; matched text
(define (match!) (set! mtext yytext))

(define (matcher yes) (if yes (begin (match!) (yylex) #t) #f))
(define (expect str)  (yytext=? str))
(define (.number)  (yytype? 'number))
(define (.id) (yytype? 'id))


;;(define (expect str) (if (yytext=? str) (begin (match!) (yylex) #t) #f))





(define (.yytext txt) (printf "yytext in ~a = ~a\n" txt yytext) #t)
;;(define (hopes x) (if (yytext=? x) (begin (yylex) #t) #f))



  
(define (out . args)
  (lambda ()
    (each arg args
          (if (eq? arg '**)
              (display mtext)
              (display arg))
          (display " "))
    ;;(apply display args)
    ;;(newline)
    #t))



(define-syntax puff
  (syntax-rules ()
    ((_ expr)
     (cond
       [(string? expr) (expect expr)]
       [(procedure? expr) (expr)]
       [else expr]))
    ((_ expr others ...)
     (begin
       (puff expr)
       (puff others ...)))))

(define-syntax-rule ($ test body ...)
  (begin
    (while (puff test)
           (and (puff body ...)))
    #t))

(define (do-it init user-data)
  (pin! user-data)
  (yylex)
  (init)
  #t)


(define-syntax-rule (pout target  outputs ...)
  (let ()
    (define ok
      (cond
        [(string? target) (yytext=? target)]
        [(procedure? target) (target)]
        [else (raise "pout is confused")]))
    (when ok
      (match!)
      ((out  outputs ...))
      (yylex))
    ok))



(define (bracket) (and (expect "(")  (ex1) (expect ")") ))
(define (ex3) (or (pout .number "LD" **) (bracket)))
(define (ex2) (puff ex3 ($ "*" (pout ex3 "MLT" "\n")) #t))
(define (ex1) (puff ex2 ($ "+" (pout ex2 "ADD" "\n")) #t))




;;(do-it ex1 "( 22+23+30)  * ( 24 + 25 )")





(define (M.EX1)  
  (pout .id "(pout " ** ")"))


(define (M.ST)
  (pout .id "(define (" ** ")")
 
  (pout "=")
  ;; (.yytext "M.ST")
  (M.EX1)
  (pout ".," ") statement ended\n"))

(define (0+ pat)
  (if (pat)
      (0+ pat)
      #t))

(define (M.PROG)
  (pout ".SYNTAX"  "YES-SYN" ) (pout .id  "PROGRAM NAME IS " ** "\n")
  ;;(while (M.ST) #t)
  (0+ M.ST)
  (pout ".END" "\nAnd we're done\n"))
  
(do-it M.PROG ".SYNTAX PROGRAM

PROGRAM  =  ST .,
ST = BAZ .,
.END")

