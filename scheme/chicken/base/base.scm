;;; BUILD A USEFUL BASE CHICKEN SCHEME INTERPRETER

;;; READLINE SUPPORT
;;; hints on readline are available at
;;; http://www.call-with-current-continuation.org/eggs/readline.html
(use readline)
(use regex)
(current-input-port (make-gnu-readline-port ""))



;;; REPL CREATION
(repl)
