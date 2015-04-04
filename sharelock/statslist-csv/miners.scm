;;;; we ask the question - for the Footsie, what percentage of
;;;; market capitalisation is in the mining/resource sector

;;(load "miners.scm")

(require-extension numbers)
(require-extension statistics)
(require-extension utils)
(require-extension srfi-6)
(require-library srfi-28)
(import (prefix format format:))
(require-extension srfi-42)

(use mccsl)

(define (read-sharelock-csv file-name)
  (define input (read-all file-name))
  (define lines (string-split input "\r" #t))
  (define field-names  (string-split (car lines) "," #t))
  (define data-lines (cdr lines))
  (set! data-lines (drop-right data-lines 1)) ; last lines is duff
  (define (splitter line)
    (define fields (string-split line "," #t))
    (set! fields (map (lambda (x) (string-delete #\" x)) fields))
    ;; last field is duff, so exclude it
    (drop-right fields 1))
  (set! data-lines (map splitter data-lines))
  ;; create an assocation list between field names and values
  (define result
    (list-ec (:list rec data-lines)
             (map list field-names rec)))
  result)

(define table (read-sharelock-csv "~/Downloads/StatsList.csv"))


(define (percentage sectors)
  ; determine what percentage of the Footsie is in given sector
  (define sector-total 0.0)
  (define total 0.0)
  (over record table
        (define (v name) (second (assoc name record)))
        (define mkt (string->number (v "MarketCap" )))
        (inc total mkt)
        (define sector (v "Sector"))
        (when (in? sector sectors) (inc sector-total mkt)))
  (print sectors (/ sector-total total)))


(percentage '("MINING"))

(percentage '("MINING" "OIL AND GAS PRODUCERS" 
              "OIL EQUIPMENT - SERVICES AND DISTRIBUTION"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(write-line "Looking at PER spread")

(define (v field-name)
  (lambda (row) (string->number (second (assoc field-name row)))))

(define pers (map (v "PER") table))
;; TODO combine the commonality with stats.scm

(setv! pers sort <)
(define (poutput i v)
  (format:format #t "P~5A  ~8,2F~%" i (exact->inexact v)))

;;(print pers)
(do-ec  (: i 5 100 5)
        (poutput i (percentile pers i)))
