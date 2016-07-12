001000 IDENTIFICATION DIVISION.
      * 2016-06-26 created by mcarter
      * shell reads shell.cob and echos it to output
001010 PROGRAM-ID. shell.

002000 ENVIRONMENT DIVISION.
002010 INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           select fd-in assign to ws-fd-in-name                         12
                   organisation is line sequential.

003000 DATA DIVISION.
003010 FILE SECTION.
       FD fd-in.                                                        1
       01 inline pic x(80).

003020 WORKING-STORAGE SECTION.
       01 ws-fd-in-name pic x(50).                                      2

003030 LOCAL-STORAGE SECTION.
003040 LINKAGE SECTION.

004000 PROCEDURE DIVISION.
       program-begin.
           move "shell.cob" to ws-fd-in-name.                           2
           display "=== ECHOING FILE ==="
           open input fd-in.
          
           

           perform forever 
           read fd-in
                   at end exit perform
                   not at end display inline
           end-read
           end-perform


           display "=== FINISHED ==="
           close fd-in.

       program-done.
           stop run.
