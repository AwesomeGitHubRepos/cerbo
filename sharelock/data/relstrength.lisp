;; relative strength
;; computer the size and median relative strength of companies in StatsList.csv
;; assumes sector is in third column, and rel strength is in fourth column


;; *** TODO ***
;; I should use a lot of the infrastructure now supplied in the standard packaging
;; see opm.lisp for an example

(ql:quickload "lili")
(ql:quickload "group-by")
(ql:quickload "cl-csv")
(ql:quickload "parse-number")

;; (lili:package-symbols 'cl-csv)
(setf csv-contents (lili:slurp-file "~/Downloads/StatsList.csv"))
(defparameter a (cl-csv:read-csv csv-contents))

(defparameter data 
  (loop for el in (cdr a) collect
        (cons (third el) 
              (parse-number:parse-real-number (fourth el)))))

(defparameter groups 
  (loop for el in (group-by:group-by data :key #'car)
        collect (list (lili:pad-right (car el) 20) 
                      (length (cdr el))
                      (lili:median (cdr el)))))

(defparameter sorted
  (sort groups (lambda (a b) (lili:test a b :key #'third :test #'<))))

(format t "~%~{~{~A ~3D ~6,2F~%~}~}" sorted)