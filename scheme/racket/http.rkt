#lang racket

;;;; useing an http client

#| Simple web scraper
(require net/url net/uri-codec)
(define (let-me-google-that-for-you str)
  (let* ([g "http://www.google.com/search?q="]
         [u (string-append g (uri-encode str))]
         [rx #rx"(?<=<h3 class=\"r\">).*?(?=</h3>)"])
    (regexp-match* rx (get-pure-port (string->url u)))))

|#

(require net/url)
(require racket/port)

(define pre #f)
(set! pre  "http://uk.finance.yahoo.com/d/quotes.csv?s=")
(set! pre "http://download.finance.yahoo.com/d/quotes.csv?s=")
(define sym #f)
(set! sym "ULVR.L")
(set! sym "^FTSE")
(define post "&f=sl1d1t1c1ohgv&e=.csv")

(define u (string-append pre sym post))
(display u)

(display (port->string (get-pure-port (string->url u))))

