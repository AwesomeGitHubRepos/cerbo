;;(load "vas-parser.scm")
(require-extension srfi-14)

;; needing installation:
(require-extension srfi-34)
(require-extension loop)
(require-extension miscmacros)
;;(load-library loop)

(import mcutils)

#|
(define (between? x lo hi)
  (and (<= lo x) (<= x hi)))

(define (lower? c) (between? c #\a #\z))
(define (upper? c) (between? c #\A #\Z))
|#

(define (digit? c)
  (if (char? c)
      (char-set-contains? char-set:digit c)
      #f))
  

(define (letter+digit? c)
  (char-set-contains?  char-set:letter+digit c))

(define (not/letter+digit? c)
  (not (letter+digit? c)))



(define (nonalphanum? c)
  (not (alphanum? c)))

(define (white? c)
  (char-set-contains?  char-set:whitespace c))


(define (nonwhite? c) (not (white? c)))

(define-syntax-rule (spanning pred sp c)
  (let ((result '()))
    (let loop ()
      (when (pred c)
	      ;;(display c)
	      (set! result (cons c result))
	      (set! c (read-char sp))
	      (loop)))
    (list->string (reverse result))))


(define (build-instruction  sp c)
  (spanning white? sp c)

  ;;(displayln "ins")
  (define instruction (spanning nonwhite? sp c))
  (spanning white? sp c)

  (define regs (make-vector 2 0))
  (define reg-index 0)
  (define offset 0)
  (define (next-char) (set! c (read-char sp)) c)
  (let loop ()
    (unless (eof-object? c)
	    (cond
	     [(eq? c #\R)
	      (vector-set! regs reg-index (next-char))
	      (set! reg-index (+ 1 reg-index))]
	     [(digit? c)  (set! offset (spanning digit? sp c))]
	     [else (next-char)])
	    (loop)))
  ;;(display c)
  (list instruction regs offset))


;;(parse-line "\t     lDA  2000(R2),R5")

(define (build-label sp c)
  (list->string (loop 
   until (or (eof-object? c) (eq? c #\:))
   collect c
   do (set! c (read-char sp)))))


(define (parse-line line)
  (define sp (open-input-string line))
  (define c (read-char sp))
  (if (eof-object? c)
      '()
      (case c
	[(#\\ #\newline) '()] ; it's a comment or blank
	[(#\tab #\space) (build-instruction sp c)]
	[else (build-label sp c)])))

