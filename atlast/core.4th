1 constant r/o \ read-only flag

file $include.fd
\ not re-entrant
: $INCLUDE ( str --)
	r/o $include.fd fopen \ -- flag
	not if abort" "Cannot include file\n" then
	$include.fd fload
	$include.fd fclose
	if abort" "FLOAD fail\n" then
;	
