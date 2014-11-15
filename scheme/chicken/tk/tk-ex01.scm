(require-extension tk)

(start-tk)

(define hello-button (tk 'create-widget 'button))

(tk/pack hello-button)

(hello-button 'configure #:text 'Howdy!)

(hello-button 'configure #:command 
    (lambda () (print "Greetings Earthlings")))


(event-loop)
(exit)
