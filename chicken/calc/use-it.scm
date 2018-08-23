;;(load "use-it.scm")
(require-extension miscmacros)
(require-extension posix)
(include "lex-out.scm")



(define (mylexer output-port)
  (lexer-init 'port (open-input-file "input.txt"))
  (let loop ()
    (define tok (lexer))
    (when (not (eq? tok 'MY-EOF))
	    (write tok output-port)
	    (newline)
	    (loop))))

(mylexer (current-output-port))
