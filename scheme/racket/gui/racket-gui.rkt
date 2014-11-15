#lang racket/gui

; Make a frame by instantiating the frame% class
(define my-frame (new frame% [label "Example"]))
  
; Make a static text message in the frame
(define msg (new message% [parent my-frame]
                 [label "No events so far..."]))
  
; Make a button in the frame
(define my-button (new button% [parent my-frame]
                       [label "Click Me"]
                       ; Callback procedure for a button click:
                       (callback (lambda (button event)
                                   (send msg set-label "Button click")))))
  
; Show the frame by calling its show method
(send my-frame show #t)