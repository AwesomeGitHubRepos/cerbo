;;;; common routines

(require-library format)
(import (prefix format fmt:))

(use mccsl)

(define (int-dir) 
  (string-append 
   (get-environment-variable "NOKILLI")
   "/sharelock/int"))

(define (int-sub-dir subdir)
  (string-append (int-dir) "/" subdir))

(define (int-sub-file subdir file-name)
  (string-append (int-sub-dir subdir) "/" file-name))

(define (calcs-dir) (int-sub-dir "calcs"))
(define (calc-filename epic) (int-sub-file "calcs" epic))

(define (cards-dir) (int-sub-dir "cards"))

(define (load-calcs)
  (define calc-files (list-directory (calcs-dir) #t))
  (map rep-read-file-lines calc-files))

(define (filter-records pass? records)
  (defcol result col)
  (safely-over rec records
               (define (v key) (second (assoc key rec)))
               (when (pass? v) (col rec)))
  result)

(define (tabulate-output records field-names)
  (define n 0)
  (newline)
  (define (print-val v) (fmt:format #t "~7,@A " v))
  (define (print-fields vals)
    (over vs vals (print-val vs))
    (newline))

  (print-val "N")
  (print-fields field-names)

  (safely-over rec records
               (define (v key) (second (assoc key rec)))
               (define vals (map v field-names))
               (inc n)
               (print-val n)
               ;;(print vals)
               (print-fields vals))
  #t)



(define (get-n n alist start)
  (filter number? (take (drop alist (- start 1)) n)))

(define (get10 alist start) (get-n 10 alist start))
(define (get11 alist start) (get-n 11 alist start))
(define (get12 alist start) (get-n 12 alist start))
;;  (filter number? (take (drop alist (- start 1)) 11)))

(define (nth alist i)
  (list-ref alist (- i 1)))

(define-simple-syntax (ce exp)
  (catch-errors -666 exp))

(define-simple-syntax (ce2 exp)
  (ce (round2 exp)))

(define (read-card epic)
  (define card-file-name (int-sub-file "cards" epic))
  (define lines (rep-read-file-lines card-file-name))
  (define data (thirds lines))
  data)

(define (rate r) (float (* 100 (- r 1))))

;;; cache the calcs if needed - we only want to load them a maximum of once
(define loaded-calcs (make-parameter #f))
(define (get-calcs)
  (let ((result (loaded-calcs)))
    (unless result
            (write-line "Loading calcs")
            (loaded-calcs (load-calcs))
            (set! result (loaded-calcs)))
    result))
;;(calcs) (loaded-calcs)

(define (filter-and-tabulate pass? field-names)
  (define passes (filter-records pass? (get-calcs)))
  (tabulate-output passes field-names)
  (newlines 2))

