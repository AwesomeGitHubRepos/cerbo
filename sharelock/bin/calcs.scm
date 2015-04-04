#!/usr/bin/csi -script
;#!/usr/bin/env csi -script
;;;(load "calcs.scm")
;;;(process-all-cards)

;;; Typical usage:
;;;    csi calcs.scm epic BATS
;;;    csi calcs.scm all


(use low-level-macros)
(require-extension list-utils)
(require-extension logical-combinators)
(require-extension statistics)
(use directory-utils)
;;; (use srfi-13)

(require-library format)
(import (prefix format format:))

(use mccsl)




(load (string-append 
       (get-environment-variable "NOKILLI") 
       "/sharelock/sharelock.scm"))

(write-line "Running calcs.scm")


(define (strictly-monotonic-increasing? alist)
  (define list1 (butlast alist))
  (define list2 (cdr alist))
  (define tests (map > list2 list1))
  ;;(print tests)
  (apply andf tests))

;;(strictly-monotonic-increasing? '(1 2 3 3 4)) ; => #f
;;(strictly-monotonic-increasing? '(1 2 3 30 40)) ; => #t
;;(strictly-monotonic-increasing? '(30 20 40)) ; => #f



(define (greet) (write-line "Greetings from calcs.scm"))

(define-macro (make-defs data)
  (with-injected (getv g11 g12 m11 hist-pers epss)
                 `(begin
                    (define (,getv n) (nth ,data n))
                    (define (,g11  n) (get11 ,data n))
                    (define (,g12  n) (get12 ,data n))
                    (define (,m11  n) (ce2 (median (,g11 n))))
                    (define ,hist-pers (,g11 20))
                    (define ,epss (,g11 527))
                    #t)))


(define (calc1 data)
  (make-defs data)
  ;(define (getv n) (nth data n))
  ;(define (g11 n) (get11 data n))
  ;(define (m11 n) (ce2 (median (g11 n))))
  (let* (;(hist-pers (g11 20))
         (sorted-hist-pers (sort hist-pers <))
         (perc (lambda (p) (percentile sorted-hist-pers p)))
         (cur-per (getv 31))
         (s0107 (ce2 (/ (getv 424) (getv 423))))
         ;(epss (g11 527))
         (divs (g12 610))
         (num-shares (g11 385))
         (nshg (ce2 (/ (last num-shares) (first num-shares)))))
    (define output "") 
    (define (output1 a b c)
      (define line (format:format #f "~5A ~9,@S ~S~%" a b c))
      ;(display line)
      (string-append! output line))
    (output1 "EPIC"  (getv 1) "EPIC")
    (output1 "NAME"  (getv 1160) "Company name")
    (output1 "SECT"  (getv 1161) "Sector")
    (output1 "INDEX" (getv 3) "INDEX")
    (output1 "DIVR" (ce2 (spearman divs)) "Rank of dividends per share")
    (output1 "DIVI" (strictly-monotonic-increasing? divs) "Dividends strictly increase?")
    ;;(write hist-pers)
    (output1 "PER0" cur-per "Curr PER")
    (output1 "PE20" (perc 20) "PER10 P20")
    (output1 "PE50" (perc 50) "PER10 P50")
    (output1 "PE80" (perc 80) "PER10 P80")
    (output1 "RREV" s0107 "REL CHANGE IN REVENUE")
    (output1 "OPM1"  (getv 73) "OPM at begining")
    (output1 "OPMM"  (m11 73) "OPM median")
    (output1 "YLD"   (getv 44) "Yield (%)")
    (output1 "Z"     (getv 343) "Z Score")
    (output1 "MKT"   (getv 369) "MKT CAP (£m)")
    (output1 "ROE0"  (getv 797) "Current ROE")
    (output1 "ROE10" (m11 112) "ROE median")
    (output1 "SP"    (getv 1456) "Share Price (p)")
    ;;(print epss)
    (output1 "GRATE" (ce2 (rate (graham-rate epss))) "Graham EPS growth rate%")
    (output1 "XRATE" (ce2 (rate (xrate epss))) "Exp-fit EPS growth rate%")
    (output1 "SREPS" (ce2 (spearman epss)) "Spearman rank of EPSa")
    (output1 "UPD" (getv 1455) "Updated on Sharelock")
    (output1 "EPS1" (getv 527) "EPS at time 1")
    (output1 "EPG01" (ce2 (/ (nth epss 11) (nth epss 1))) "EPS11/EPS1")
    (define epgs (g11 541)) ; earnings growths
    (output1 "EPG02" (length (filter (lambda (x) (<= x -5)) epgs))
             "#Times EPSG <= -5%")
    (output1 "EE" (getv 213) "EV/EBITDA")
    (output1 "NSHG" nshg "GROWTH IN NUM SHARES")
    output))



(define (process-epic epic)
  (define data (read-card epic))
  (define output (calc1 data))
  (define calc-file (calc-filename epic))
  (spit calc-file output)  
  #t)

(define (process-all-cards)
  (define epics (list-directory (cards-dir)))
  (over epic epics 
        (handle-exceptions
         exn (begin (format:format #t "~%~A Failed~%" epic))
         ;;(let ()
           
           ;;(display ".")
           (flush-output)
           (process-epic epic)))
  (newline))
  
;;(process-all-cards)
;;(write (process-epic "ULVR"))
  

  

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (main)
  (write-line "Running main"))

(format:format #t "Args: ~S~%" (command-line-arguments))

(define cmd (catch-errors "" (car (command-line-arguments))))
(when (string=? "all" cmd) (process-all-cards))
(when (string=? "epic" cmd) 
      (process-epic (second (command-line-arguments))))


;;(calc1)




