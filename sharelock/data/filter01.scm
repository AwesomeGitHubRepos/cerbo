;;;; (load "filter01.scm")

(use srfi-1)
(require-library format)
(import (prefix format fmt:))

;;(use mccsl)

(load "~/repos/nokilli/sharelock/sharelock.scm")



;;(define calcs (load-calcs))






(write-line "Filter: PER0 LE 16 and ROE0 GE 15 and RREV GE 1.1")
(filter-and-tabulate 
 (lambda (v) (and (<= (v 'PER0) 16)
                  (>= (v 'ROE0) 15)
                  (>= (v 'RREV) 1.1)))
 '(EPIC PER0 ROE0 RREV))
;;(newlines 2)



(write-line "Filter: Champions per http://bit.ly/GYbGch")
(filter-and-tabulate 
 (lambda (v) (and (>= (v 'OPMM) 15)
                  (>= (v 'ROE10) 15)
                  (>= (v 'SREPS) 0.9)))
 '(EPIC OPMM ROE10 SREPS))
;;(newlines 2)
  
(write-line "Filter: XRATE>3%, GRATE>3%, YLD>0, Sorted by descending yield")
(write-line "Use it for a F958B style rating")
(define passes 
  (filter-records 
   (lambda (v) (and (> (v 'GRATE) 3.0)
                    (> (v 'XRATE) 3.0)
                    (> (v 'YLD) 0)
                    #t))
           
   (get-calcs)))
;;(print calcs)
;;(print passes)
(define (yld<? a b)
  (define (yld x) (catch-errors 0 (float (second (assoc 'YLD x)))))
  (> (yld a) (yld b)))
(define sorted (sort passes yld<?))
;;(pretty-print sorted)
(tabulate-output sorted '(EPIC XRATE YLD))
(newlines 2)
;;(exit)

(write-line "Filter: EPS at least doubled over last decade, Max 2 earnings dip below 5%")
(filter-and-tabulate 
 (lambda (v) (and (>= (v 'EPS01) 2)
                  (<= (v 'EPG01) 2)
                  #t))
 '(EPIC OPMM ROE10 EPS01 EPG01 PER0 YLD))