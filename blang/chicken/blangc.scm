;;(load "blangc.scm")
(require-extension lalr-driver)

(cond-expand
 (compiling (declare (uses grammar-out)))
 (else (load "grammar-out.scm")))


(define (my-make-token sym val)
  (make-lexical-token sym 0 val))
(define (simple-token sym)
  (my-make-token sym sym))


(cond-expand
 (compiling (declare (uses lex-out)))
 (else (include "lex-out.scm")))

;;(lexer-init 'port (open-input-file "input.txt"))
(lexer-init 'string "14+12+5")

(print (eval (blang-parser lexer print)))

