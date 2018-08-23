;;(load "calc.scm")
(require-extension lalr-driver)
(include "calc.yy.scm")

(define (my-make-token sym val)
  (make-lexical-token sym 0 val))

(include "lex-out.scm")

(lexer-init 'port (open-input-file "input.txt"))
;;(lexer-init 'string "14+12+5")

(print (calc-parser lexer print))
