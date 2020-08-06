libc$ = "libc.so.6"
sub getchar()
	ch =  foreign_function_call(libc$, "int", "getchar") 
	return ch
end sub


while (getchar() > -1) 
	print chr$(ch);
	rem getchar()
	rem print ch
	rem if ch>-1 print chr$(ch) ;
wend
