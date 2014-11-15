;;; foo.scm

; The declaration marks this source file as dependant on the symbols provided
; by the bar unit:
(declare (uses bar))

(write (fac 10)) (newline)
