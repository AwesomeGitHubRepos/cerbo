(require-library format)
(import (prefix format format:))
(use mccsl)
(load "~/repos/nokilli/sharelock/sharelock.scm")


(define (process-card epic)

  (define data (read-card epic))

  (define years (get10 data 387))
  (define aeps-s (get10 data 514)) ; adjusted eps
  (define divs (get10 data 584))
  (define nav0 (nth data 710))
  (define nav nav0)

  (define (process-year year eps div)
    (define retained (- eps div))
    (define croe (* 100 (/ eps nav))) ; clean ROE
    (define dcov (/ eps div))
    (fmt:format #t "~A ~8,2F ~8,2F ~8,2F ~8,2F ~8,1F ~8,2F~%"  
                year nav eps div retained croe dcov)
    ;;(newline)
    (inc nav retained))
  
  (define (print-table)
    (newline)
    (fmt:format #t "~A - ~A ~%" epic (nth data 1147))
    (display "YEAR")
    (over header '(NAV EPS DIV RETAIN ROE% DCOV)
          (fmt:format #t " ~8,@A" header))
    (newline)
    (set! nav nav0)
    (map process-year years aeps-s divs)
    #t)
  
  (print-table)
  #t)

(process-card "VTC")

;;(fmt:format #f "*~7,2F*" 123.4567)