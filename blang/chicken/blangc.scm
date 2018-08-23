;;(load "blangc.scm")
(require-extension lalr-driver)

(cond-expand
 (compiling (declare (uses calc-yy)))
 (else (load "calc-yy.scm")))
;;(import calc.yy)

(define (my-make-token sym val)
  (make-lexical-token sym 0 val))
(define (simple-token sym)
  (my-make-token sym 0 sym))


(cond-expand
 (compiling (declare (uses lex-out)))
 (else (include "lex-out.scm")))

(lexer-init 'port (open-input-file "input.txt"))
;;(lexer-init 'string "14+12+5")

(print (calc-parser lexer print))

