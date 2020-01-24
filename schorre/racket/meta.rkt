#lang racket
;(require racket/stream)

(define desc "
.SYNTAX PROGRAM
ST = .ID .LABEL * '=' EX1 '.,' .,
PROGRAM = '.SYNTAX' .ID $ ST .END .,
.END
")

(define pin (open-input-string desc))
(define (pc) (peek-char pin))
(define (rc) (read-char pin))

(define (crange? c lo hi) (and (char>=? c lo) (char<=? c hi)))
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


(define (yylex)
  (accum white?) ; eat white-space
  (cond
    [(char=? #\' (pc)) (list 'str (get-string))]
    [(letter? (pc)) (list 'id (accum non-white?))]
    [(non-white? (pc)) (list 'lit (accum non-white?))]  
    [else 'dunno]))

(yylex) (yylex) (yylex) (yylex)
