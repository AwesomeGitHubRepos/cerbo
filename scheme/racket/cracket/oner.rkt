#lang racket

;;;; cracket - "Carters Racket library":

;;;; a twitter-like clone for the japanese tsunami
;;;; iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-port 8080
;;;; http://forums.fedoraforum.org/showthread.php?p=1451424#post1451424




;;; required libraries
(require web-server/formlets
         web-server/servlet
         web-server/servlet-env)


;;; some helper functions

(define-syntax catch-errors
  (syntax-rules ()
          ((_ error-value body ...)
           (with-handlers ([exn:fail? (lambda (exn) error-value)])
             body ...)))) ; catch-errors

(define-syntax safely 
  (syntax-rules ()
    ((_ body ...)
     (catch-errors "Error" body ...)))) ; safely

(define-syntax defines
    (syntax-rules ()
      ((defines a) (define a 'undefined))
      ((defines a b ...) (begin (defines a) (defines b ...)))))

(define (as-number x)
  (if (number? x)
      x
      (safely (string->number x))))
(define (as-string x)
  (if (string? x)
      x
      (safely (number->string x))))



(define vat%
  (class object%
    (super-new)
    (init-field (amount 100) (rate 17.5) (which "gross") (request 'undefined))
    
    (define/public (calculations)
      (defines net vat gross net-string vat-string gross-string)
      (if (equal? which "gross")
          (begin
            (set! gross amount)
            (set! net (safely (/ gross (+ 1 (/ rate 100)))))
            (set! vat (safely (- gross net))))
          (begin
            (set! net amount)
            (set! vat (safely (/ (* rate net) 100.0)))
            (set! gross (safely (+ net vat)))))
      (set! net-string (as-string net))
      (set! vat-string (as-string vat))
      (set! gross-string (as-string gross))
      (values net-string vat-string gross-string)) ;calculations
    
    (define/public (reset-inputs request)
      (define bindings (request-bindings request))
      (define (get-number sym)
        (define v1 (extract-binding/single sym bindings))
        (define v2 (string->number v1))
        (if (number? v2) v2 v1))
      (set! amount (get-number 'amount))
      (set! rate (get-number 'rate))
      (set! which (extract-binding/single 'which bindings))) ; reset-inputs
    
    (define/public (inputs-as-strings)
      (define checked-gross '(dummy ""))
      (define checked-net '(dummy ""))
      (if (equal? which "gross")
            (set! checked-gross '(checked ""))
            (set! checked-net '(checked "")))
      (values (as-string amount) (as-string rate) checked-gross checked-net)) ; inputs-as-strings
      
    ))
;;; our "VAT" web-page itself
;;; STATE is whatever we choose to manage outselves, in whatever form.

(define (vat-page vat request) 
  
  (define (button-clicked request)
    (send vat reset-inputs request)
    (vat-page vat (redirect/get)))
    
  (define (response-generator embed/url)
    (define-values (amount-string rate-string checked-gross checked-net) 
      (send vat inputs-as-strings))
    (define-values (net-string vat-string gross-string) (send vat calculations))
    
    
    
    ;; return our rendering of the results
    `(html (head (title "VAT Calculator"))
           (body
            (h1 "Simple VAT calculator")
            
            (h2 "Inputs")
            (form ([action ,(embed/url button-clicked)])
                  (table (tbody
                          (tr (td "Amount:") 
                              (td (input [ (type "text") 
                                           (name "amount") 
                                           (value ,amount-string) ]))
                            
                              (td (input [ (type "radio")
                                           (name "which")
                                           (value "gross")
                                           ,checked-gross ])
                                  "Gross")
                              (td (input [ (type "radio")
                                           (name "which")
                                           ,checked-net ])
                                  "Net"))
                          (tr (td "VAT Rate%:") 
                              (td (input [ (type "text") 
                                           (name "rate") 
                                           (value ,rate-string)]))
                              (td "")
                              (td ""))))
                  (input [(type "submit") (name "") (value "Calculate")]))
            
            (h2 "Outputs")
            (table (tbody
                    (tr (td "Net:") (td ((align "right")) ,net-string))
                    (tr (td "VAT:") (td ((align "right")) ,vat-string))
                    (tr (td "Gross:") (td ((align "right")) ,gross-string))))
            
            (p "Enjoy!"))))
   
  (send/suspend/dispatch response-generator))

(define (kickoff req)
  (kickoff
   (send/suspend
    (lambda (k-url)
      (response/xexpr
       `(html (body (a ([href ,k-url]) "Hello world!"))))))))

;;; Start the server
(define (go)
  (define log-to (build-path (find-system-path 'home-dir) 
                             "racket-server-access.txt"))  
  (serve/servlet kickoff #:port 8080  
                 #:servlet-path "/"
                 #:servlet-regexp #rx".*" 
                 #:listen-ip #f #:log-file log-to))

(go)



