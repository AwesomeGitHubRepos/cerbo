#lang racket
  
(require racket/port)
(require net/url)

(provide http-get)
(define (http-get url-as-string)
  (port->string (get-pure-port (string->url url-as-string))))
