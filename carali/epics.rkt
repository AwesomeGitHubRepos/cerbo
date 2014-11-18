#lang racket

(provide *prices* *prices-file* cache-price download/cache-price download/cache-uncached-price
         fetch-price get-price
         load-prices
         match-epics
         price? price price-amount
         price-cached?
         price-epoch price-epic price-epic-date=? price-epic-date<?
         price-epic<? price-epoch<? price-today
         print-all-prices print-price print-prices
         reset-prices save-prices set-prices sort-price-cache sort-prices)

(require srfi/26)
(require srfi/48)
(require (prefix-in srfi19: srfi/19))

(require racket/file)
(require racket/path)
(require racket/serialize)

(require carali/datetime)
(require carali/formatting)
(require carali/http)
(require carali/lists)
(require carali/misc)

;(print "i run")





(define (yahoo-fetch pre epic post)
  (define url (string-append pre epic post))
  (http-get url))

(provide yafi)
(define (yafi epic)
  (let* ((response (yahoo-fetch "http://download.finance.yahoo.com/d/quotes.csv?s=" 
                                epic "&f=sl1d1t1c1ohgv&e=.csv"))
         (fields (regexp-split #rx"," response))
         (price-field (second fields))
         (price (string->number price-field)))
    (* 0.01 price)))



; GB0003875100.L is Fidelity Special Situations
(require (planet "html-parsing.rkt" ("neil" "html-parsing.plt" 1 2)))
(require carali/lists)

(provide oeic)
(define (oeic epic)
  (define response (yahoo-fetch "http://uk.finance.yahoo.com/q?s=" epic "&ql=0"))
  (define xml (html->xexp response))
  (define html (extract-tree '(3 6 3 4 5 3 2 3 2 2 2 2 1 1 2) xml))
  (define val (string->number html))
  val)

  


(provide print-yafi-line)
(define (print-yafi-line fetcher epic)
  (define d (srfi19:date->string  (srfi19:current-date) "~5"))
  (define p (fetcher epic))
  (formatln "yafi ~14F ~A ~20,4F" epic d p))
  
  


















(serializable-struct price (epic epoch  amount))

(define *prices-dir* (build-path (find-system-path 'home-dir) ".config" "racket"))
(define *prices-file* (build-path *prices-dir* "yafi.dat"))
(define *prices* null) ; always kept in code/date order


(define (fetch-price epic)
  (let ((p (yafi epic)))
    (price epic (epoch-today) p)))

(define (reset-prices) (set-prices null))

(define (save-prices)
  (unless (directory-exists? *prices-dir*)
    (make-directory *prices-dir*))
  (with-output-to-file *prices-file*
    (lambda () (write (serialize *prices*)))
    #:exists 'replace))

(define (load-prices)
  (reset-prices)
  (when (file-exists? *prices-file*)
    (with-input-from-file *prices-file*
      (lambda () (set! *prices* (catch-errors null (deserialize (read))))))))

;(load-prices) ;; auto-load epics


(define (print-price price)
  (when (price? price)
    (displayln (format "~6F ~a ~a" 
                       (price-epic price) 
                       (epoch->string (price-epoch price)) 
                       (price-amount price)))))
;(print-price (get-price "SHG.L"))
;(print-price (get-price "SHG.L" #:pred <= #:when (epoch-today)))

(define (set-prices v)  (set! *prices* v))



(define (price-epic-date=? price1 price2)
  ;; do the prices PRICE1 and PRICE2 have the same epic and date?
  (and (equal? (price-epic price1) (price-epic price2))
       (= (price-epoch price1) (price-epoch price2))))

(define (cache-price p)
  ;; add a price to cache
  (set! *prices* (filter-not (λ (pr) (price-epic-date=? p pr)) *prices*))
  (addend! *prices* p)
  #t)

(define (download/cache-price epic)
  (let ((p (fetch-price epic)))
    (cache-price p)
    p))


(define (price-epic<? price1 price2)
  (string<? (price-epic price1) (price-epic price2)))

(define (price-epoch<? price1 price2)
  (< (price-epoch price1) (price-epoch price2)))

(define (price-epic-date<? price1 price2)
  (if (price-epic<? price1 price2)
    #t
    (if (price-epic<? price2 price1)
      #f
      (if (price-epoch<? price1 price2)
          #t
          #f))))

(define (sort-prices prices)
  (sort prices price-epic-date<?))

(define (sort-price-cache)
  (sort! *prices* price-epic-date<?))


(define (print-prices prices #:heading (heading null))
  (when heading (displayln heading))
  (unless (empty? prices)
    (for-each print-price prices)))

(define (print-all-prices #:heading (heading null))
  (print-prices *prices* #:heading heading))

(define (price-today epic)
  (let ((p (download/cache-uncached-price epic)))
    (price-amount p)))

(define (match-epics prices code)
  (filter-by price-epic prices  string=? code))

(define (get-price code #:pred [pred <=] #:when [epoch-when (epoch-today)])
  (define found null)
  (each p *prices*
        (when (string=? code (price-epic p))
          (when (pred (price-epoch p) epoch-when)
            (set! found p))))
  found) ; might reutnr null
;; Equivalent examples:
;; (print-price (get-price "SHG.L"))
;; (print-price (get-price "SHG.L" #:pred <= #:when (epoch-today)))


(define (price-cached? epic epoch)
  (let* ((p1 (price epic epoch 0.0))
         (match? (lambda (x) (price-epic-date=? x p1))))
  (has *prices* match?)))

(define (download/cache-uncached-price epic)
  (if (price-cached? epic (epoch-today))
      (get-price epic)
      (download/cache-price epic)))