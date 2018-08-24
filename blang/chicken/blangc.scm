;;(load "blangc.scm")
(require-extension lalr-driver)
(require-extension fmt)

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


(define (prlist x)
  (fmt #t x nl))
(define (just x)
  (prlist x))


(lexer-init 'port (open-input-file "input.txt"))
;;(lexer-init 'string "JUST 14+12+5;")
(define syntax-tree (blang-parser lexer print))
(print syntax-tree)
(eval syntax-tree)


