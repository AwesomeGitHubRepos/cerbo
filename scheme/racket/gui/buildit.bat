set destdir=%HOMEPATH%\racketgui
raco exe --gui racket-gui.rkt
raco distribute %destdir% racket-gui.exe
del racket-gui.exe
