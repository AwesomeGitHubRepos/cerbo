;;(load "vas-parser.scm")
(require-extension srfi-13)
(require-extension srfi-14)

(require-extension srfi-1)

(require-extension regex)
;; needing installation:
;;(require-extension srfi-34)
;;(require-extension loop)
;;(require-extension miscmacros)
;;(require-extension matchable)
(require-extension holes)
;;(require-extension anaphora)

(use regex-literals)
(require-extension regex-literals)

(import mcutils)

#|
(string-match #/^(\d{2}):(\d{2})(..)/ "11:59pm")
("11:59pm" "11" "59" "pm")

(string-match #/^\s+(\w{3})\s+(\d+)\(R(\S)\),R(\S).*/
	      "\t     LDA  2000(R2),R5")

(string-match #/^\s+(\w{3})\s+(\d*)\(R(\S)\),(\d*)R(\S).*/
	      "\t     LDA  R2,R5")
|#

;; set the messy regex used by instruction
(define op-base "\\s+(\\w{3})\\s*")
(define reg "(\\d*)(\\(?R(\\w)\\)?)?")
(define instruction-regex  (string-append op-base reg ",?" reg ".*"))

(define (instruction str)
  (define grps (string-match instruction-regex str))
  (write grps)
  (define (nth n) (list-ref grps n))
  
  (define rx (nth 4))
  (define ry (nth 7))

  (define offset 0)
  (define (set-offset pos)
    (define val (nth pos))
    (when (> (string-length val) 0)
	  (set! offset (string->number val))))
  (set-offset 2) ; offset for rx
  (set-offset 5) ; offset for ry

  (list rx ry offset))


(define (carf lst) (if (null? lst) #f (car lst)))

(define (parse-line line)
  (define in (string->list line))
  (case (carf in)
    [(#f #\\ #\newline) '()]
    [(#\tab \space)
     (instruction in)]
    [else ; label
     (break (@> eq? #\:) in)]))
  
(file->lines "test.vas")

;;(parse-line "foobar12:")
