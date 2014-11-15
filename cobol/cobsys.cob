*> demonstrate a call to system

identification division.
program-id. hack-asynch.
data division.
working-storage section.
01 result pic s9(9).
procedure division.
display "Sleep for 2".
call "SYSTEM" using "echo hello"
                     returning result.
display "Result: " result.

stop run.