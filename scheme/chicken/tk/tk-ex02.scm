(use tk)
(start-tk)
(tk/pack
  (tk 'create-widget 'button
      #:text 'Howdy!
      #:command (lambda () (print "Greetings Earthlings!")))
  (tk 'create-widget 'button
      #:text 'Exit
      #:command (lambda () (end-tk)))
  #:expand #t
  #:fill 'both)
(event-loop)
(exit)
