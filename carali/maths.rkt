#lang racket  ;-*-Scheme-*-

(provide atanh avg exp-fit float flow gain% inc integers least-squares 
         median npv percentile rebase xirr xirr-flows)

           
           
(require plot)
(require racket/date)
(require srfi/26)
(require data/queue)

(require racket/generator)
(require unstable/contract) ; non-empty-string?
(require rnrs/io/ports-6)

(require carali/lists)
(require carali/datetime)
(require carali/misc)

;(require (planet "main.rkt" ("gfb" "structured-loops.plt" 2 0)))


(provide sum)
(define (sum numbers)
  (foldl + 0 numbers))

(define-syntax inc
  ;;; increment in place
  ;;; Example:
  ;;; (define a 10)
  ;;; (inc a) ; adds 1 to a
  ;;; (inc a 2)
  ;;; a ; => 13 (= 10 + 1 + 2)
  (syntax-rules ()
    ((inc var val)
     (let ()
       (set! var (+ var val))
       var))
    ((inc var) (inc var 1))))

(define (atanh x)
  ;arctanh 
  (* 0.5 (log (/ (+ 1 x) (- 1 x)))))

(define (avg numbers)
  (let ((len (length numbers))
        (sum (apply + numbers)))
    (float (/ sum len))))


(define float exact->inexact)



;; Generate integers, by by successive calls
;; Example:
;; (let ((i (integers)))
;;(for/list ((j '(10 11 12)))
;;  (cons (i) j)))
;; => '((0 . 10) (1 . 11) (2 . 12))  
(define (integers)
  (let ((n 0))
    (lambda () 
      (set! n (+ 1 n))
      (- n 1))))

(define (quasi-position vec pos)
  (let* ((position (lambda (x) (inexact->exact (floor (* pos x)))))
         (len (vector-length vec))
         (p1 (position (- len 1)))
         (v1 (vector-ref vec p1))
         (p2 (position len))
         (v2 (vector-ref vec p2)))
    (avg (list v1 v2))))



(define (median numbers)
  ; Example
  ;(median '(3 1 5 2 4)) ; => 3.0
  ;(median '(3 1 4 2)) ; => 2.5
  ;(median '(0.36 0.44 0.43 0.60 0.64 0.73 0.61 0.49 0.32 0.36))
  (let ((sorted-vector (list->vector (sort numbers <))))
    (quasi-position sorted-vector 0.5)))


(provide mean)
(define (mean numbers)
  ;return mean of a list of numbers
  (float (/ (foldl + 0 numbers) (length numbers))))
;(mean '(234 23 46)) ;=> 101

(provide x-xbar^2)
(define (x-xbar^2 numbers)
  ; sum of (x - xbar)**2
  (define xx (sum-prod numbers numbers))
  (define xbar (mean numbers))
  (- xx (* (length numbers) xbar xbar)))

(provide stdevs)
(define (stdevs numbers)
  ;standard deviation of a sample
  (define num (x-xbar^2 numbers))
  (define n-1 (- (length numbers) 1))
  (sqrt (/ num n-1)))
;(stdevs '(1345 1301 1368 1322 1310 1370 1318 1350 1303 1299))
; result is 27.46391572
; agrees with http://office.microsoft.com/en-us/excel-help/stdev-HP005209277.aspx


(define (percentile value  numbers)
  ;; determine where a value sits in a list of numbers
  (let* ((sorted-numbers (sort numbers <))
         (len (length numbers))
         (gt (lambda (x) (> value x)))
         (place (length (filter gt sorted-numbers))))
    (float (/ place len))))

; (percentile 10 '(16.3  	 24.2  	 25.7  	 15.9  	 12.6  	 11.3  	 10.6  	 9.1  	 8.6  	 9.5  	))



(define (rebase a-list)
  (let ((m (apply min a-list)))
    (map (lambda (x) (- x m)) a-list)))
;(rebase '( 5 4 6))  

(provide pv)
; Compute the present value of a flow FLOW at a time T and rate R
(define (pv r t flow)
  (/ flow (expt (+ 1 r) t)))


; DEF (npv r times payoffs)
; Return the net present value of a list of PAYOFFs occuring at TIMES for a discount rate R
; Example 
; (npv 0.1 '(0 1 ) '(-10 11)) ; => 0  
(define (npv r times payoffs)
  (let ((result 0.0))
    (for ((t times)
          (p payoffs))
         (inc result (pv r t p)))
    result))



(define (xirr times payoffs)
  ; return the rate r which gives an NPV closest to 0
  ;  Example 
  ;   (irr '(0 1) '(-10 11)) ; => 0.1 (i.e. 10%)
  ;(irr '(0 0.5 1) '(-100 -100 231)) ; => 0.21 (i.e. 21%)
  (define npvs (for/list ((r100 (in-range -99 100)))
                 (define r (float (/ r100 100)))
                 (cons r (npv r (rebase times) payoffs))))
  (define (val x) (abs (cdr x)))
  (define (ord x y) (< (val x) (val y)))
  (define sorted-npvs (sort npvs ord))  
  (define best (first sorted-npvs))
  (define result (* 100.0 (car best)))
  result)

(require srfi/13)
(provide xirr-a)
(define (xirr-a)
  ;imported in cli
  ;read stdin. Each line in stadin should contain a DATE in the form:
  ;YYYY-DD-MM FLOW
  ;xirr-a will print the XIRR of this data
  ;for example usage, see tests xirr-a.bat
  
  ;decode input
  (define times null)
  (define payoffs null)
  (let loop ()
    (define line (read-line))
    (unless (equal? eof line)   
      (set! line (string-trim-both line))
      (whence line
              (define fields (regexp-split #rx"[ \t]+" line))
              ;(displayln fields)
              (addend! times (ymd->years (first fields)))
              (addend! payoffs (string->number (second fields)))
              #t)
      (loop)))
    
  (displayln (xirr times payoffs))
  (void))


; calculate the xirr on a list of pairs 
; (xirr-b '( ( "2010-01-01" -100)  ( "2010-12-31" 120)))
(provide xirr-b)
(define (xirr-b alist)
  (let* ((times (rebase (map (lambda (x) (ymd->years (first x))) alist)))
         (flows (map second alist)))
    (print times)
    (print flows)
    (xirr times flows)))


(provide sum-prod)
(define (sum-prod list-a list-b) (sum (map * list-a list-b)))


; source: http://www.efunda.com/math/leastsquares/lstsqr1dcurve.cfm
; Find least squares fit y = a + b x
; also return R, the correlation coefficient, and R^2, the "coefficient of determination".
; These calculations have been verified again R on 11-Dec-2010
(define (least-squares list-of-x list-of-y)
  (define (sum lst) (apply + lst))
  (define sum-x (sum list-of-x))
  (define sum-y (sum list-of-y))
  (define sum-xx (sum-prod list-of-x list-of-x))
  (define sum-xy (sum-prod list-of-x list-of-y))
  (define sum-yy (sum-prod list-of-y list-of-y))
  (define n (length list-of-x))
  (define (cd-ef a b c d) (- (* a b) (* c d)))
  (define denom (cd-ef n sum-xx  sum-x sum-x))
  (define num-a (cd-ef sum-y sum-xx sum-x sum-xy))
  (define a (/ num-a denom))
  (define num-b (cd-ef n sum-xy sum-x sum-y))
  (define b (/ num-b denom))
  (define r-denom (sqrt (* denom (cd-ef n sum-yy sum-y sum-y))))
  (define r (/ num-b r-denom))
  (define r2 (* r r))
  (hash 'a a 'b b 'r r 'r2 r2))

; exponentially fit a time series
; yvals is list of values
; For example usage, see:
; http://alt-mcarter.blogspot.com/2010/12/cyclicals-and-defensive-shares.html
(define (exp-fit  yvals)
  (define (range n) 
    (let loop ((lst null) (x (- n 1)))
      (if (>= x 0) 
          (loop (cons x lst) (- x 1))
          lst)))
  (define x (range (length yvals)))
  (define y (map log yvals))
  (define linear-fit (least-squares x y))
  (define (val v) (hash-ref (least-squares x y) v))
  (define (exp-val sym) (exp (val sym)))
  (define rate (exp-val 'b))
  (define intercept (exp-val 'a))
  (define terminal (* intercept (expt rate (last x))))
  (hash 'intercept intercept 'rate rate 'r2 (val 'r2) 'terminal terminal))

#|
(provide  firsts seconds thirds)
;;(define (nths alist idx0) (map (lambda (x) (list-ref x idx0)) alist))
(define (firsts alist) (nths alist 0))
(define (seconds alist) (nths alist 1))
(define (thirds alist) (nths alist 2)) 
(provide fourths fifths sixths sevenths)
(define (fourths alist) (nths alist 3))
(define (fifths alist) (nths alist 4))
(define (sixths alist) (nths alist 5))
(define (sevenths alist) (nths alist 6))
|#

(define (range-len  alist)
  ; return a list of length the same size as ALIST
  (let loop ((result '()))
    (if (< (length result) (length alist))
        (loop (cons (length result) result))
        (reverse result))))

(provide correlation-coefficient)
(define (correlation-coefficient list-of-x list-of-y)
  (define (sum lst) (apply + lst))
  (define sum-x (sum list-of-x))
  (define sum-y (sum list-of-y))
  (define sum-xx (sum-prod list-of-x list-of-x))
  (define sum-xy (sum-prod list-of-x list-of-y))
  (define sum-yy (sum-prod list-of-y list-of-y))
  (define n (length list-of-x))
  (define (cd-ef a b c d) (- (* a b) (* c d)))
  (define denom (cd-ef n sum-xx  sum-x sum-x))
  (define num-a (cd-ef sum-y sum-xx sum-x sum-xy))
  (define a (/ num-a denom))
  (define num-b (cd-ef n sum-xy sum-x sum-y))
  (define b (/ num-b denom))
  (define r-denom (sqrt (* denom (cd-ef n sum-yy sum-y sum-y))))
  (define r (/ num-b r-denom))
  r)


(define (correlation-coefficient-test)
  ;;http://easycalculation.com/statistics/learn-correlation.php
  (print (correlation-coefficient
          '(60 61 62 63 65)
          '(3.1 3.6 3.8 4 4.1))))
;;(correlation-coefficient-test)

(define (square x) (* x x))


(provide spearman)
(define (spearman inputs)
  (define iotas (range-len inputs))
  (define pairs (map list iotas inputs))
  (define sorted (sort pairs (lambda (x y) (< (second x) (second y)))))
  (define remapped (firsts sorted))
  (define r (correlation-coefficient iotas remapped))
  (float r))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; smat - the maths repl

(define (decode-input str)
  (set! str (regexp-replace* "," str ""))
  (define seq (regexp-split #px"[[:space:]]+" str))
  (map (lambda (x) (if (string->number x) (string->number x) x))
       (filter non-empty-string? seq)))


(require racket/pretty)
(provide smat-stats)
(define (smat-stats numbers)
  (define results (encons (list 'length (length numbers)
                                'mean   (mean numbers)
                                'median (median numbers)
                                'spearman (spearman numbers)
                                'spearman-squared (square (spearman numbers))
                                'sorted (sort numbers <)
                                'stdevs (stdevs numbers)
                                'exp-fit (exp-fit numbers)
                          )))
  (pretty-display results))
  
(define (smat-repl in)
  (displayln "(smat) to re-enter loop, REPL to drop into repl, DIE to kill program")
  (let loop ()
    (display "Numbers: ")
    (flush-output)
    (unless (port-eof? in)
      (define inputs (decode-input (read-line in)))
      (when (empty? inputs) (loop))
      (define i0 (first inputs))
      
      (when (equal? i0 "die") (exit))
      (unless (equal? i0 "repl")           
        ;(newline)
        (displayln inputs)
        (smat-stats inputs)
        
        (newline)
        (loop)))))


(provide stats-repl)
(define (stats-repl [in (current-input-port)])
  (catch-errors (begin (displayln "Error!") (smat-repl in))
                (smat-repl in)))
;(smat (open-input-string " hellow world")) 
;(smat (open-input-string " 1,7.4 	25,.4 	15.7 	13.7 	19.4 	20.9 	")) 

;; smat end
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (gain% num denom)
  (* 100.0 (- (/ num denom) 1.0)))

(struct flow (epoch amount))

(define (xirr-flows flows)
  (let* ((flow-list (queue->list flows))
         (times (map flow-epoch flow-list))
         (amounts (map flow-amount flow-list)))
    ;(print times)
    ;(print amounts)
    (xirr times amounts)))

(provide between?)
(define (between? low x hi)
  (and (<= low x) (<= x hi)))