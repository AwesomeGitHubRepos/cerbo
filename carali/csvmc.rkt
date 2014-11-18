#lang racket  ;-*-Scheme-*-


(require 2htdp/batch-io)

(provide read-csv-file-as-hashes)
(define (read-csv-file-as-hashes filename)
  (define csv1 (read-csv-file filename))
  (define headers (car csv1))
  (define data (cdr csv1))
  (define (hash-row row) 
    (define hsh (make-hash))
    (for ((h headers)
          (v row))
         (hash-set! hsh h v))
    hsh)  
  (define hashed-data (map hash-row data))
  hashed-data)
