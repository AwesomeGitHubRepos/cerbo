#lang racket

(provide cache-ref load-cache save-cache set-cache)

(require racket/serialize)

(require carali/misc)

(define *cache* null)
(define (reset-cache) (set! *cache* (make-hash)))
(reset-cache)
(define *cache-dir* (build-path (find-system-path 'home-dir) ".config" "racket"))
(define *cache-file* (build-path *cache-dir* "cache.dat"))

(define (load-cache)
  (reset-cache)
  (when (file-exists? *cache-file*)
    (with-input-from-file *cache-file*
      (lambda () (set! *cache* (catch-errors null (deserialize (read))))))))

(define (save-cache)
    (unless (directory-exists? *cache-dir*)
    (make-directory *cache-dir*))
  (with-output-to-file *cache-file*
    (lambda () (write (serialize *cache*)))
    #:exists 'replace))

(define (set-cache key value)
  (hash-set! *cache* key value))

(define (cache-ref key )
  (hash-ref *cache* key))
