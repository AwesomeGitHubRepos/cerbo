;;; fetching code from the internet

(use http-client)

;; this works 07-Apr-2011:
;;(display (with-input-from-request "http://wiki.call-cc.org/" #f read-string))

;; try with a redirect from yahoo finance
;; doesn't work at 07-Apr-2011 due to redirect
(define url 
    "http://uk.finance.yahoo.com/d/quotes.csv?s=ULVR.L&f=sl1d1t1c1ohgv&e=.csv")
(display (with-input-from-request url #f read-string))

(display "Finished")
(exit)

