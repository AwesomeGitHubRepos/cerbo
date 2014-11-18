#lang racket

;hashes manipulation

(provide hash+)
(define (hash+ h key v)
  (define curr (hash-ref h key 0))
  (hash-set! h key (+ v curr)))
