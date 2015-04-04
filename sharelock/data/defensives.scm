;(require-extension srfi-1)
(require-extension mccsl)
(use srfi-42)
(use symbol-utils)

(write-line "defensives.scm")
(define sc
  (with-input-from-file 
      "~/repos/nokilli/sharelock/int/misc/epics.scm"
    read))

;; should prolly generalise to work on col names
(define defensives (firsts (filter (lambda (row) 
                                     (string=? "t" (second row))) 
                                   sc)))

(define calcs
  (map (lambda (epic)
         (define fname
           (string-append "~/repos/nokilli/sharelock/int/calcs/" epic))
         (rep-read-file-lines fname))
       defensives))


(define (passes title pass?)
  (display title)
  (display ": ")
  (define results '())
  (over vals calcs
        (define (v key) (second (assoc key vals)))
        (when (pass? v) (addend! results (v 'EPIC))))
  (print results))


(passes "cheaps" (lambda (v) (<= (v 'PER0) (v 'PE20))))
(passes "normals" (lambda (v) (and (> (v 'PER0) (v 'PE20))
                                   (< (v 'PER0) (v 'PE80)))))
(passes "expensives" (lambda (v) (>= (v 'PER0) (v 'PE80))))
                                   
(exit)
