#lang racket  ;-*-Scheme-*-

;; manipulating sharelock holmes

(require racket/file)

(require (planet "html-parsing.rkt" ("neil" "html-parsing.plt" 1 2)))

(require carali)


(define file-name (build-path (find-system-path 'home-dir) ".config" "sharelock" "htm" "ulvr.htm"))
(define html(file->string file-name))
(define xml (html->xexp html))
(enumerate-tree xml)

;(extract-tree '(3 6 3 4 5 3 2 3 2 2 2 2 1 1 2) xml)


