#lang racket ; -*- scheme -*-


(require srfi/1)
(require racket/function)

(provide addend addend! cdrs encons enumerate-tree extract-tree has map! 
         pairwise remove-if-not sort!)

(require carali/hashes)
(require carali/misc)


(define (has alist pred )
  (let/ec return
    (for-each (lambda (el)
                (when (pred el) (return #t)))
              alist)
    (return #f)))
;(has '(1 3) (lambda (x) (= x 2))) => #f
;(has '(1 3) (lambda (x) (= x 1))) => #t


(define (enumerate-tree node (path null) (num 0))
  (when node
    (if (list? node)
        (for ([el node]
              [i (in-range (length node))])
          (enumerate-tree el (append path (list num)) i))
        (begin
          (print (cdr (append path (list num))))
          (newline)
          (print node)
          (newline)
          (newline)))))
  

(define (extract-tree path tree)
  (let ((tree (list-ref tree (car path))))
    (if (empty? (cdr path))
        tree
        (extract-tree (cdr path) tree))))

#|
 process a list ALIST pairwise, assigning the first value of the pair to VAR1 and 
the second value to VAR2, and then process the body.
Example:
(pairwise v1 v2 '(1 2 3 4)
          (display (format "First: ~a  Second: ~a" v1 v2))
          (newline))
Outputs:
First: 1  Second: 2
First: 3  Second: 4
|#
(define-simple-syntax (pairwise var1 var2 alist body ...)
  (let loop ((pairs (take-by 2 alist)))
    (unless (empty? pairs)
      (let ((cur (car pairs)))
        (let ((var1 (car cur))
              (var2 (cadr cur)))
          (begin body ...))
        (loop (cdr pairs))))))

(define (encons alist)
  (map (lambda (x) (cons (first x) (second x)))
       (take-by 2 alist)))
;;(encons '(1 2 3 4)) ; => ((1 . 2) (3 . 4))
;;(encons '(1 2 3)) ; => ((1 . 2) (3))

(define (remove-if-not func alist)
  (filter (lambda (x) (not (func x))) alist))
; suspicious: might be wrong - should prolly just use filter-not instead

(define (addend alist el)
  (append alist (list el)))

(define-simple-syntax (addend! var el)
  (set! var (addend var el)))

(define-syntax sort!
  (syntax-rules ()
    ((sort! var rest ...)
     (set! var (sort var rest ...)))))
;; destructive sort
;(define a '(5 4))
;(sort! a <) 
;(print a) => (4 5)

;;(define-simple-syntax (map! lst func)
;;  (set! lst (map func lst)))

(define (cdrs lst)
  (map cdr lst))
;obtain all the cdrs in a list


; DEF (take-by n lst)
; Group a list LST into a list of lists containing N elements, dropping any elements at the end that wont fit into N elements.
; Example 
; (take-by 3 '(1 2 3 4 5 6 7 8 9 10 11 12 13)) ; => '((1 2 3) (4 5 6) (7 8 9) (10 11 12))
(provide take-by)
(define (take-by n lst)
  (let loop ( (front null) (back lst) )
    (if (>= (length back) n)
        (loop (append front (list (take back n)))
              (drop back n))
        front)))

(provide filter-by)
(define (filter-by key lst pred v)
  (filter (lambda (x) (pred (key x) v)) lst))


(provide filter-not-by)
(define (filter-not-by key lst pred v)
  (filter-not (lambda (x) (pred (key x) v)) lst))

(provide each)
(define-simple-syntax (each var alist body ...)
  (for-each (lambda (var) body ...) alist))
#| Example:
(each i '(1 2 3)
      (print (+ 1 i))
      (newline))
prints:
2
3
4
|#

(provide sum-categories)
(define (sum-categories alist)
  (define h (make-hash))
  (each row alist
        (hash+ h (car row) (cdr row)))
  h)


; (nths '((2 3) (4 5) (6 7)) 0) => '(2 4 6)
(provide nths)
(define (nths alist n)
  (map (lambda (x) (list-ref x n)) alist))



; (firsts '((2 3) (4 5) (6 7))) => '(2 4 6)
(provide firsts)
(define (firsts alist) (nths alist 0))

; (seconds '((2 3) (4 5) (6 7))) => '(3 5 7)
(provide seconds)
(define (seconds alist) (nths alist 1))

(provide grouping)
(define (grouping alist extract-key)
  ;;(define (sector x) (hash-ref x "Sector"))
  (define keys (delete-duplicates (map extract-key alist)))
  (for*/list ((el keys)) (filter (lambda (x) (equal? el (extract-key x))) alist)))

(provide buckets)
(define (buckets n alist)
  (let* ((l (length alist))
         (size (floor (/ l n)))
         (remainder alist))
    (for/list ((b (range n)))
              (define front (if ( < b (- n 1)) (take remainder size) remainder))
              (set! remainder (drop remainder size))
              front)))
#| split a list into N buckets. e.g.
(buckets 5 '(1 2 3 4 5 6 7 8 9 10)) => '((1 2) (3 4) (5 6) (7 8) (9 10))
(buckets 3 '(1 2 3 4 5 6 7 8 9 10)) => '((1 2 3) (4 5 6) (7 8 9 10))
Any "leftovers" go into the last bucket
|#
