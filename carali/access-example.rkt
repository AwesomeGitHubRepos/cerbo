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
