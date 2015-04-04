;;;; (load "filter02.scm")

;;(use srfi-1)
;;(require-library format)
;;(import (prefix format fmt:))

(load "~/repos/nokilli/sharelock/sharelock.scm")

(write-line "Filter: Monotonically increasing dividends, ROE0 >15")
(filter-and-tabulate 
 (lambda (v) (and (= 1 (v 'DIVR))
                  (> (v 'ROE0) 15)))
 '(EPIC PER0 ROE0 RREV))
