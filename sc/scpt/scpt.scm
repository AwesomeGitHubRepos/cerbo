;;; compile using bigloo
;;; for an excellent introduction, see
;;; http://www-sop.inria.fr/mimosa/fp/Bigloo/doc/bigloo-12.html#Regular-parsing

(module sc (main start))


(define *scpt-debug* #f)

(define (-A c) (+ 1 (- (char->integer c) (char->integer #\A))))
(define (string->column str)
  (let ((char-list (string->list str)))
    (if (= 1 (length char-list))
	(-A (car char-list)) ; single char
	(let* ((c1 (car char-list))
	       (a1 (-A c1))
	       (c2 (cadr char-list))
	       (a2 (-A c2)))
	  (+ (* 26 a1) a2)))))

      


(define (inspect what value)
  (when *scpt-debug*
	(display (format "--->~a: ~a ~%" what value))))

(define result '()) ; global - yuk

(define lalr
(lalr-grammar
    (nl  col end equal int leftstring letx plus mult minus div const lpar rightstring rpar stringx)



(contents
 ((lines) (set! result lines))
 (() (display "empty")))

(lines
 ;((nl lines) lines)
 ((line)  line)
 ((line lines) (if line (cons line lines) lines )))

(line
 ((expression nl) (inspect 'expression expression) expression )
 ((assignment nl) (inspect 'assignment assignment) assignment )
 ((nl) #f ))


(assignment
 ((letx cell equal expression) (list 'let cell expression))
 ((leftstring cell equal stringx) (list 'leftstring cell stringx))
 ((rightstring cell equal stringx) (list 'rightstring cell stringx)))

(cell ((col int) (list 'cell col int)))
 
(expression
 ((expression plus term)   (list 'add expression term))
 ((expression minus term)  (list 'sub expression term))
 ((term)                   term))

(term
 ((term mult factor)       (list 'mul term factor))
 ((term div factor)        (list 'div term factor))
 ((factor)                 factor))

(factor
 ((lpar expression rpar)   expression)
 ((const)                  const)
 ((int)                    int)
 ((cell)                   cell))))

(define regular
  (regular-grammar 
   ()
   ((+ (or #\tab #\space)) (ignore))
   ((: #\# (* all)) (ignore)) ; ignore comments beginning with #
   ((: "goto" (* all))    (ignore)) ; we can ignore the goto statements
   (#\newline              'nl)
   ((: (+ digit) "." (+ digit))             (cons 'const (string->number (the-string))))
   ((+ digit)      (cons 'int (string->number (the-string))))    
   ("let"                    'letx)
   ("leftstring"             'leftstring)
   ("rightstring"            'rightstring)
   ("end"                    'end) ; Not part of sc - this is just a convenience
   ((+ alpha )    (cons 'col (string->column (the-string))))
   
   ((: #\" (* (out #\")) #\") (cons 'stringx (the-substring 1 (-fx (the-length) 1))))
   (#\+                    'plus)
   (#\=                    'equal)
   (#\-                    'minus)
   (#\*                    'mult)
   (#\/                    'div)
   (#\(                    'lpar)
   (#\)                    'rpar)))

(define (parser)
  (set! result '())
  (read/lalrp lalr regular (current-input-port))
  (reset-eof (current-input-port))
  result)


(define (start argv)
  (write (parser)))

  
