0 prompt

\ case statement implemented as per jonesforth

\ DEFINE WORDS

: ?DUP dup if dup then ;

: CASE  0 ; immediate

: OF 
	postpone  OVER 
	postpone = 
	postpone IF
	postpone DROP 
; immediate

: ENDOF  postpone ELSE	; immediate

: ENDCASE 
	postpone DROP 

	BEGIN
		   ?DUP  
	WHILE
		postpone THEN
	REPEAT
; immediate


\ NOW USE THEM
: x
	3 case
		2  of ." a" endof
		3  of ." b" endof
		." unhandled case" cr
	endcase
;

." Expect: b" cr
x cr
