CARALI - CArter's RAcket LIbrary

As from 23-Nov-2012
   raco link ~/repos/tacc/carali


As from 04-Sep-2011:
Set-up collect:
   cd c:\Users\mcarter\repos\tacc\carali
   raco link %cd%
Compile the documentation:
   raco setup


Old as at 04-Sep-2011:
One-time setup of collects:
   mkdir %APPDATA%\Racket\5.1.3\collects
As administrator:
   mklink /D %APPDATA%\Racket\5.1.3\collects\carali %USERPROFILE%\repos\tacc\scheme\carali
#Compile the documentation after you have changed it:
#   raco setup carali
Compile the documentation:
    raco setup scribblings/main
But maybe it should be 
    raco setup
file:///C:/Racket/doc/scribble/how-to-doc.html?q=help#%28part._setting-up%29


   
Some miscellaneous foo that you probably shouldn't do:
; raco planet link mcarter carali.plt 1 0 c:/Users/repos/tacc/scheme/carali
; raco planet link mcarter carali.plt 1 0 c:\Users\mcarter\repos\tacc\scheme\carali
;raco planet unlink mcarter carali.plt 1 0
; raco planet remove mcarter carali.plt 1 0
; raco planet install mcarter carali.plt 1 0


To use the maths functions (e.g. median):
   (require carali/maths)
   (median '(1 2 3)) ;=> 2
