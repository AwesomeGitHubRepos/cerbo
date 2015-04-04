(load "calcs.scm")
;;(use low-level-macros)

(define data (read-card "ABM"))
(make-defs data)
epss
(define epgs (g11 541))
(define epgA (length (filter (lambda (x) (< x -0.05)) epgs)))
(define output (calc1 data))
(ce2 (/ (nth epss 11) (nth epss 1)))
epgs
(process-epic "ABM")