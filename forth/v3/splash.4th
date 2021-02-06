0 prompt
\ create a fancy splash screen
\ playing with ansi escape sequences

\ http://ascii-table.com/ansi-escape-sequences.php

: ESC[ 		27 emit [char] [ emit ; \ begin escape sequence. 27 is ESC

: GOTOLC	swap esc[ %d [char] ; emit  %d [char] f emit ; \ goto line and column ( l c -- )
: CLS		esc[ "2J" type ; \ erase display and move cursor to home

: 5SPS	space space space space space ;
: 10SPS	5SPS 5SPS ;
: 20SPS 10SPS 10SPS ;
: 30SPS 20SPS 10SPS ;
: 40SPS 20SPS 20SPS ;
: 60SPS 20SPS 20SPS 20SPS ;
: 70SPS 60SPS 10SPS  ;

30 constant black
31 constant red
32 constant green
33 constant yellow
34 constant blue
35 constant magenta
36 constant cyan
37 constant white
39 constant default

\ : NUM	[char] 0 + emit ; \ output a digit
: FG	ESC[ 0 %0nd "m" type ; \ set foreground colour
: BG	10 + fg ; \ set background colour

: NL default bg cr ;
cls
: blue-line blue bg 60SPS nl ;
blue-line blue-line
: line-1 blue bg 30SPS   red bg 40SPS nl ;
line-1 
: chunk blue bg 5SPS green bg 30SPS red bg 30SPS 5SPS nl ;
: 5chunks chunk chunk chunk chunk chunk ;
5chunks 5chunks \ 5chunks
line-1 line-1
blue-line
blue-line
blue-line

\ ESC[ "42m" type \ green background
\ 5 10 gotolc
\ "hello" type 
\ 7 11 gotolc "world" type
\ cr
\ .s


