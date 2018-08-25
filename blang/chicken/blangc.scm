;;(load "blangc.scm")
(require-extension lalr-driver)
(require-extension fmt)
(require-extension chili)

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


;; TODO should be part of chili
(define-syntax-rule (do-list var lst body ...)
  (begin
    (define (loop-1 lst1)
      (when (pair? lst1)
	    (define var (car lst1))
	    body ...
	    (loop-1 (cdr lst1))))
    (loop-1 lst)))
#|
 Example usage:
(do-list i '(10 11)
	 (print i)
	 (print i))
|#

(define (prlist x)
  (do-list i x (fmt #t i))  
  (newline))
;; (prlist '(10 11))

(define (just x)
  (prlist x))


(define (go)
  (lexer-init 'port (open-input-file "if.txt"))
  ;;(lexer-init 'string "JUST 14+12+5;")
  (define syntax-tree (blang-parser lexer print))
  (displayln "Made it this far")
  (pretty-print syntax-tree)
  (eval syntax-tree)
  #t)
(go)


