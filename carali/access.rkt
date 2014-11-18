#lang racket  ;-*-Scheme-*-

;; MS Access manipulation

 (require mysterx)

 (provide connection-close connection-create connection-open
          recordset-close recordset-create recordset-empty? recordset-field-value
          recordset-loop-lambda recordset-open recordset-move-first recordset-move-next)


#| Example

#lang racket
(require carali/access)
(define conn (connection-create))
(connection-open conn
            "PROVIDER=Microsoft.Jet.OLEDB.4.0;DATA SOURCE=C:/Users/mcarter/tracker/tracker.mdb;")
(define rs (recordset-create))
(recordset-open rs "select * from tblProjects" conn 1 3)
(recordset-loop-lambda rs (lambda (rs)
                            (display (recordset-field-value rs "name"))
                            (newline)))
(recordset-close rs)
(connection-close conn)

|#

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; connection

(define (connection-close conn)
  (com-invoke conn "Close"))

(define (connection-create)
  (cocreate-instance-from-coclass "ADODB.Connection"))

(define (connection-open connection connection-string)
  (com-invoke connection "Open" connection-string))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; recordset

 
(define (recordset-close rs)
  (com-invoke rs "Close"))

(define (recordset-create)
  (cocreate-instance-from-coclass "ADODB.Recordset"))

(define (recordset-empty? rs)
  (or (com-get-property rs "BOF") (com-get-property rs "EOF")))
  
(define (recordset-field-value rs field-name)
  (let ((field (com-get-property rs "Fields" `("Item" ,field-name))))
    (com-get-property field "Value")))

(define (recordset-loop-lambda rs function)
  (when (not (recordset-empty? rs))
    (recordset-move-first rs)
    (let loop ()
      (when (not (recordset-empty? rs))
        (function rs) ; does the bulk of the work
        (recordset-move-next rs)
        (loop)))))

(define (recordset-open rs sql conn arg1 arg2)
  ; prolly use arg1 = 1, arg2 = 3
  (com-invoke rs "Open" sql conn arg1 arg2))


(define (recordset-move-first rs) (com-invoke rs "MoveFirst"))

(define (recordset-move-next rs) (com-invoke rs "MoveNext"))

  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  