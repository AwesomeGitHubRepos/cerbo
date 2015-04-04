;;; categorise Operating Margins from Cap200

(ql:quickload "sharelock")
(in-package :sharelock)




(let* ((csv (read-stats-list-csv))
       (operating-margin-text (get-csv-column csv "Operating_Margin"))
       (operating-margins (as-floats operating-margin-text)))
  (terpri)
  (write-line "Operating Profit Margins in deciles")
  (print-ntiles 10 operating-margins))