;;;; Place where users can define sets of epics that they are interested in

(ql:quickload "sharelock")
(use-package :sharelock)
;; defensives
(defparameter defensives
  '(AZN BAG BATS BSY CRDA DPH DGE DPLM DNO DOM EXPN GRG GSK HIK HLMA 
        IMI IMT JHD MTO MRW PZC RB. RR. RTN SAB SBRY SGE
        SHP SN. TLPR TSCO ULVR VCT VOD WTB))

(setf defensives (mapcar #'symbol-name defensives))

(dolist (epic defensives) (fetch-and-card epic))

(dolist (epic defensives)
  (print epic)
  (print (rove-calculate epic))
  (terpri))

;; rejected as being undefensive
