#lang racket
;#lang racket/gui

(require racket/gui)

;(rename-in srfi/48 (format some-other-name))
; [13:31] <DT``> or (except-out racket/base format)
(require (rename-in srfi/48 (format format48)))

(require carali/misc)

(require fiqua/about)


; Make a frame by instantiating the frame% class
(define my-frame (new frame% [label "Fiqua - Financial Quant"]))


;;; menu
(define menu-bar (new menu-bar% [parent my-frame]))
(define main-menu (new menu% [label "Main Menu"] [parent menu-bar]))
(define help-menu (new menu% [label "Help"] [parent menu-bar]))
(define menu-about (new menu-item% [label "About"] [parent help-menu] 
                        [callback (lambda (i e) (show-about-box my-frame))]))


  
;;; input parameters

(define (make-param the-label cb)
  (let ((hp (new horizontal-pane% [parent my-frame])))
    (new message% [label the-label] [parent hp] [min-width 100])
    (new text-field% [parent hp] [label ""] [min-width 100] [callback cb]))
  (void))

(define-simple-syntax (make-callback var)
  (lambda (i e)
    (set! var (catch-errors #f (string->number (send i get-value))))))

(define-simple-syntax (def-param var label)
  (define var #f)
  (make-param label (make-callback var)))


;(define ta (make-param "Total Assets"))
(def-param rev "Revenue")
(def-param ebit "EBIT")
(def-param ca "Current Assets")
(def-param ta "Total Assets")
(def-param cl "Current Liabilities")
(def-param tl "Total Liabilities")
;(def-param wc "Working Capital")
(def-param re "Accumulated retained earnings")
(def-param mc "Market Cap")
(define zones (new message% [parent my-frame] [label "z <1.1 = Danger . z>2.6 = Healthy. Grey otherwise"]))
(define z-text (new message% [parent my-frame] [label "z-score: ??"] [min-width 200]))

(define (wc) (- ca cl))


(define (calculate-z fld ev)
  (define z-score (catch-errors "Invalid Input"
                              (let* ((x1 (/ (wc) ta))
                                     (x2 (/ re ta))
                                     (x3 (/ ebit ta))
                                     (x4 (/ mc tl))
                                     (z (+ (* 6.56 x1) (* 3.26 x2) (* 6.72 x3) (* 1.05 x4))))
                                (format48 "~4,1F" z))))  
  (set! z-score (format48 "z-score: ~a" z-score))
  (send z-text set-label z-score))
  



;;; calculate button
(define my-button (new button% [parent my-frame]
                       [label "Calculate"]
                       ; Callback procedure for a button click:
                       (callback calculate-z)))

  
; Show the frame by calling its show method
(send my-frame show #t)