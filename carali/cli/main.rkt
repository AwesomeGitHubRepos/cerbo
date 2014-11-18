#lang racket/base

; command-line routines
; a typical way you might use this:
; racket -l carali/cli/main COMMAND [ARG1 ...]




(require carali/maths)
(require carali/misc)

#|
(define dispatch-table
  (hash 'xirr-a xirr-a))

(define command (vector-ref (current-command-line-arguments) 0))
(set! command (string->symbol command))
(set! command (hash-ref dispatch-table command))
(command)
|#


;Extract the command as the first argument from the command line
;and execute it
;For an example, see tests\xirr-a.bat
;For an explanation of what the namespaces are about, see
;http://docs.racket-lang.org/guide/eval.html
(define-namespace-anchor a)
(define ns (namespace-anchor->namespace a))
(define command ;we obtain the firest command line argument as a symbol
  (string->symbol 
   (vector-ref (current-command-line-arguments) 0)))
(set! command (eval command ns)) ; convert the symbol into a procedure
(command) ; execute it