\ example of reading in a file
1 constant r/o \ read-only
file fd
variable buf 80 allot \ space for string
: stype ( buf n --) do dup @ emit 1+ loop drop ;
"hello.4th" r/o fd fopen \ leaves flag on stack
drop \ just forget about the flag for now
fd 80 buf fread \ #bytes read left on stack
buf swap 0 stype
\ you will need to loop around as necessary
fd fclose
