;;; about box

(module about racket
  (provide show-about-box)
  
  (require racket/gui)



  (define (show-about-box the-parent)
    (define dlg (new dialog% [label "About fiqua"] [parent the-parent]))
    (define about (new text-field% [parent dlg] [label ""] [min-width 500] [min-height 500] [style '(multiple)]))
    (send about set-value "Fledgling financial statement analyser
Calculates z-score for non-manufacturing companies.
This program is free, and COMES WITH NO WARRANTY. Please email me if you use it, as it encourages me to perform further development
Want this program to do more? I might be available for consultancy.
Author: Mark Carter
Contact: mcturra2000@yahoo.co.uk
Twitter: http://twitter.com/#!/mcturra2000
Released: 17-Jul-2011")
    (send dlg show #t)
    #t)
  
  )