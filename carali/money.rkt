#lang racket
  



;(require data/queue)
(require plot srfi/13 srfi/26 srfi/48)

(require carali/datetime)
(require carali/formatting)
(require carali/http)
(require carali/lisp)
(require carali/lists)
(require carali/maths)
(require carali/misc)
(require carali/epics)

(define (get-quote sym)
  ;; Example (get-quote "HMV.L") ; => 55.75 last trade
  (define url  (string-append "http://uk.finance.yahoo.com/d/quotes.csv?s=" sym "&f=sl1d1t1c1ohgv&e=.csv"))
  (define raw (http-get url))
    (define fields (regexp-split "," raw))
  (define last-trade-str (second fields))
  (define result (string->number last-trade-str))
  result)











(define (plot-y y-list f)
  (define xmin 0)
  (define xmax (length y-list))
  (define ymin (apply min y-list))
  (define ymax (apply max y-list))
  (define pvals
    (let ((i (integers)))
      (for/list ((y y-list))
        (vector (i) y 2))))
  
  (plot (mix (points pvals) (line f))        
        #:x-min xmin #:x-max xmax #:y-min ymin #:y-max ymax))

(define (stats sym vals)
  (display (format "~a~%" sym))
  
  (define fit (exp-fit vals))
  (define (fit-val key) (hash-ref fit key))
  (set! fit (hash-set fit 'median (median vals)))
  
  
  (define keys (sort (map symbol->string (hash-keys fit)) string<?))
  (map (lambda (k)
         (define key (string-pad-right k 10))
         (display (format "~a ~a~%" key (fit-val (string->symbol k)))))
       keys)
  (newline)
  
  
  ; this has to be last, otherwise the plot wont display
  (define intercept (fit-val 'intercept))
  (define rate (fit-val 'rate))
  (define (f x) (* intercept (expt rate x)))
  (plot-y vals f))




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; etran

(provide etran etran-epic etran-epoch etran-epoch->string etran-tag etran-tag->string 
         etran-qty etran-cost set-etran-qty! set-etran-cost!)
(provide  etrans-for-epic get-quote print-etran print-etrans set-ptag 
          stats value-today)
(provide sell-etran)

(struct etran (epic epoch tag (qty #:mutable) (cost #:mutable)))

(define (etran-epoch->string etran)
  (epoch->string (etran-epoch etran)))

(define (etran-tag->string etran)
  (symbol->string (etran-tag etran)))

(define (print-etran etran)
  (print-list (map (lambda (func) (func etran)) 
                   (list (compose1 symbol->string etran-epic) etran-epoch->string etran-tag->string  
                         etran-qty etran-cost))))

(define (print-etrans etrans)
  (print-list '("EPIC" "DATE" "TAG" "QTY" "COST"))
  (for-each print-etran etrans))

(define (value-today etran)
  (* 0.01 (price-today (etran-epic etran)) (etran-qty etran)))

(define (etrans-for-epic epic etrans)
  (filter (λ(x) (equal? epic (etran-epic x ))) etrans))
  ;(remove-if-not  (lambda (x) (string-equal epic (etran-epic x))) etrans))

(provide *portfolio*)

(define *portfolio* null)
(define *ptag* 'undefined) ; portfolio tag
(define (set-ptag sym) (set! *ptag* sym))

(provide buy-etran)
(define (buy-etran epic y m d qty cost)
  (define e (etran epic (epoch y m d) *ptag* qty cost))
  (addend! *portfolio* e)
  (void))

(define (sell-etran epic y m d qty cost)
  (buy-etran epic y m d (- qty) (- cost))
  (void))

(provide analyse-transactions)
(define (analyse-transactions list-of-etrans key)
  (let* ((dates (map etran-epoch list-of-etrans))
         (qty (apply + (map etran-qty list-of-etrans )))
         (costs (mapcar etran-cost list-of-etrans))
         (cost (apply + costs))
         (flows (mapcar value-today list-of-etrans))
         (value (apply + flows))
         (profit (- value cost))
         )

    (addend! dates (epoch-today))
    (map! dates epoch->years)
    (define amounts (addend costs (- value)))
    (define irr (xirr dates amounts))

    (encons `(:key ,key :qty  ,qty :cost ,cost :value ,value :profit ,profit :xirr ,irr))))


(provide print-portfolio)
(define (print-portfolio portfolio base-date base-index-value)
  (terpri)
  (print-list '("key" "qty" "cost" "value" "profit" "xirr"))
  (define print-portfolio-line (compose print-list cdrs))
  
  ; obtain the epics in the portfolio in alphabetical order
  (define epics (apply set (map etran-epic *portfolio*)))
  (set! epics (set->list epics))
  (sort! epics (λtk string<? symbol->string))
  
  (each sym epics
        (define etrans (etrans-for-epic sym  portfolio))
        (print-portfolio-line (analyse-transactions etrans sym))
        #t)
  (terpri)
  (print-portfolio-line (analyse-transactions portfolio "SUMM:"))

  ; comarable result for FTAS
  (define ftas-today (price-amount (download/cache-uncached-price "^FTAS")))
  
  (define times (map epoch->years `(,base-date ,(epoch-today))))
  ;(print times)
  (define amounts `(,(- base-index-value) ,ftas-today))
  (define xirr-ftas (xirr times amounts ))
  (define ftas-line (encons `(:key "^FTAS" :qty "" :cost ,base-index-value :value ,ftas-today
                                   :profit ,(- ftas-today base-index-value) :xirr ,xirr-ftas)))
  (print-portfolio-line ftas-line)
  #t)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; accounts

(provide *accounts* def-account)
(provide account account-code account-desc)

(struct account (code desc))

(define *accounts* null)

(define (def-account code desc)
  (addend! *accounts* (cons code desc)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; commodity

(provide *commodities*)
(provide commodity-id commodity-desc commodity-code commodity-downloader)
(struct commodity (id desc code downloader))

(require racket/dict)


(require carali/functional)
(define *commodities* (make-custom-hash (λtk equal? commodity-id) 
                                        (compose1 equal-hash-code symbol->string commodity-id)))

(provide def-commodity)
(define (def-commodity id desc code downloader)
  (define com (commodity id desc code downloader))
  (dict-set! *commodities* com com ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; posts


(provide *posts* make-post)

(provide addend-post! dr/cr post post-acode post-amount)

(struct post (acode amount))

(define *posts* null)

(define (addend-post! p)
  (addend! *posts* p))

(define (make-post acode amount)
  (let* ((account (find acode *accounts* #:key account-code))
         (p (post acode amount)))
    (when (null? account)
      (raise "Must have an account")) ; todo better than this
    (addend! *accounts* p)
    p))


(define (dr/cr dr cr amount)
  (addend-post! (post dr amount))
  (addend-post! (post cr (- amount))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; aggregation routines


(provide etb)

(define (etb)
  (define grand 0.0)
  (define fmt (lambda (a b) (displayln (format "~a ~10,2F" a b))))
  (each a *accounts*
        (define code (car a))
        
        (define total 0.0)
        (each p *posts*
              (when (equal? code (post-acode p))
                (inc total (post-amount p))))
        
        (fmt code total)
        (inc grand total)
        #t)
  (newline)
  (fmt 'sum grand)
  #t)
  