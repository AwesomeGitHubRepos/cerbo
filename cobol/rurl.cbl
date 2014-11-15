        *> Compile: cobc -x -free -ffunctions-all rurl.cbl
*>      $ SET SOURCEFORMAT"FREE"
IDENTIFICATION DIVISION.
PROGRAM-ID.  AcceptAndDisplay.
AUTHOR.  Michael Coughlan.
*> Uses the ACCEPT and DISPLAY verbs to accept a student record 
*> from the user and display some of the fields.  Also shows how
*> the ACCEPT may be used to get the system date and time.

*> The YYYYMMDD in "ACCEPT  CurrentDate FROM DATE YYYYMMDD." 
*> is a format command that ensures that the date contains a 
*> 4 digit year.  If not used, the year supplied by the system will
*> only contain two digits which may cause a problem in the year 2000.

ENVIRONMENT DIVISION.
INPUT-OUTPUT SECTION.
FILE-CONTROL.
   SELECT RURL-FILE ASSIGN TO "/home/mcarter/dbase/RURL.DAT"
          ORGANIZATION IS LINE SEQUENTIAL.
DATA DIVISION.
FILE SECTION.
FD RURL-FILE.
01 RURL-RECORD.
   05 UID  PIC 9(3).
   05 URL PIC X(250).
WORKING-STORAGE SECTION.
01 StudentDetails.
   02  StudentId       PIC 9(7).
   02  StudentName.
       03 Surname      PIC X(8).
       03 Initials     PIC XX.
   02  CourseCode      PIC X(4).
   02  Gender          PIC X.

*> YYMMDD
01 CurrentDate.
   02  CurrentYear     PIC 9(4).
   02  CurrentMonth    PIC 99.
   02  CurrentDay      PIC 99.

*> YYDDD
01 DayOfYear.
   02  FILLER          PIC 9(4).
   02  YearDay         PIC 9(3).


*> HHMMSSss   s = S/100
01 CurrentTime.
   02  CurrentHour     PIC 99.
   02  CurrentMinute   PIC 99.
   02  FILLER          PIC 9(4).

01 EOF-RURL-FILE PIC X VALUE 'N'.
01 NEXT-ID PIC 9(3) VALUE 0.
01 ACTION PIC X.

PROCEDURE DIVISION.
Begin.
    *> 
    DISPLAY "CREATE, QUERY, DUMP (UPATE, DELETE, DUMP)?"
    ACCEPT ACTION.
    EVALUATE ACTION
      WHEN 'C' PERFORM CREATE
      WHEN 'D' PERFORM DUMP
      WHEN 'Q' PERFORM QUERY
      WHEN OTHER DISPLAY "INPUT ERROR. ENTER ONE OF: CRD"
    END-EVALUATE
    STOP RUN.


CREATE.
    OPEN EXTEND RURL-FILE.
    DISPLAY "Enter student details using template below".
    DISPLAY "ID AND URL".
    DISPLAY "IIIUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU".
    ACCEPT RURL-RECORD.
    MOVE Trim( RURL-RECORD) TO  RURL-RECORD.
    WRITE RURL-RECORD.
    CLOSE RURL-FILE.
    

DUMP.
    OPEN INPUT RURL-FILE.
    MOVE 'N' TO EOF-RURL-FILE.
    PERFORM UNTIL EOF-RURL-FILE = 'Y'
      READ RURL-FILE 
        AT END MOVE 'Y' TO EOF-RURL-FILE
        NOT AT END
          INSPECT RURL-RECORD REPLACING ALL X'0D' BY SPACES
          DISPLAY "[" Trim(RURL-RECORD) "]"
          *> DISPLAY UID
          *> DISPLAY URL
          *> DISPLAY " "
      END-READ
    END-PERFORM.
    CLOSE RURL-FILE.

QUERY.
    DISPLAY "FIXME".
