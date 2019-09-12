fact = 1
i = 10
:loop
fact = fact * i
i = i - 1
IF i THEN GOTO :loop FI
PRINT fact
