       REPLACE ==LINE-LENGTH== BY ==1024==.

      *-----------------------------------------------------------------
      *    main procedure ----------------------------------------------
      *-----------------------------------------------------------------
       IDENTIFICATION DIVISION. 
       PROGRAM-ID. DAY02.
       AUTHOR. Andreas Wegscheider

      *-----------------------------------------------------------------
       DATA DIVISION. 

       WORKING-STORAGE SECTION. 

       01 IDX                PIC 9(4).
       01 IDX2               PIC 9(4).

       01 INSTRUCTIONS.
         02 INSTR-DATA OCCURS 128 TIMES.
           05 INSTR-LINE     PIC X(LINE-LENGTH).

       01 KEYPAD-ROWS OCCURS 3 TIMES.
         02 KEYPAD-KEYS OCCURS 3 TIMES
                             PIC X.

       01 CODE-LINE          PIC X(128).
       01 CODE-IDX           PIC 9(4).

       01 KEY-POS-X          PIC 9(2).
       01 KEY-POS-Y          PIC 9(2).

       01 INSTR-COUNT        PIC 9(4).

       PROCEDURE DIVISION .

           MOVE "789" TO KEYPAD-ROWS(1).
           MOVE "456" TO KEYPAD-ROWS(2).
           MOVE "123" TO KEYPAD-ROWS(3).

           CALL "LOAD-INSTRUCTIONS" USING INSTRUCTIONS INSTR-COUNT.

           DISPLAY "loaded " INSTR-COUNT " instructions: "


           MOVE 2 TO KEY-POS-X.
           MOVE 2 TO KEY-POS-Y.

           PERFORM VARYING IDX FROM 1 BY 1 UNTIL IDX > INSTR-COUNT 
               PERFORM VARYING IDX2 FROM 1 BY 1
                  UNTIL INSTR-LINE(IDX)(IDX2:1) = ' '
      *            DISPLAY "'" INSTR-LINE(IDX)(IDX2:1) "'"

                   IF IDX2 > LINE-LENGTH
                       DISPLAY "ERROR: line was too long. "
                               "this program cannot handle the given "
                               "input."
                       GO TO LINE-TOO-LONG
                   END-IF

                   EVALUATE INSTR-LINE(IDX)(IDX2:1) 
                   WHEN 'U'
                       IF KEY-POS-Y < 3
                           ADD 1 TO KEY-POS-Y
                       END-IF
                   WHEN 'D'
                       IF KEY-POS-Y > 1
                           SUBTRACT 1 FROM KEY-POS-Y 
                       END-IF
                   WHEN 'L'
                       IF KEY-POS-X > 1
                           SUBTRACT 1 FROM KEY-POS-X 
                       END-IF
                   WHEN 'R'
                       IF KEY-POS-X < 3
                           ADD 1 TO KEY-POS-X 
                       END-IF
                   END-EVALUATE

                   DISPLAY INSTR-LINE(IDX)(IDX2:1)
                           " ("
                           KEY-POS-X
                           ", "
                           KEY-POS-Y
                           ") "
                           KEYPAD-KEYS(KEY-POS-Y, KEY-POS-X)

               END-PERFORM
               DISPLAY KEYPAD-KEYS(KEY-POS-Y, KEY-POS-X)
               MOVE KEYPAD-KEYS(KEY-POS-Y, KEY-POS-X)
                  TO CODE-LINE(IDX:1)
           END-PERFORM.

           DISPLAY "-- END INSTRUCTIONS".

           DISPLAY "CODE IS " CODE-LINE.

           GOBACK.

       LINE-TOO-LONG.
           
           DISPLAY "line too long".

           GOBACK.

      *-----------------------------------------------------------------
      *    load instructions -------------------------------------------
      *-----------------------------------------------------------------
       IDENTIFICATION DIVISION. 
       PROGRAM-ID. LOAD-INSTRUCTIONS.

       ENVIRONMENT DIVISION. 
       INPUT-OUTPUT SECTION. 
       FILE-CONTROL. 
           SELECT INPUT-FILE ASSIGN TO "input02.txt"
              ORGANIZATION IS LINE SEQUENTIAL.

       DATA DIVISION. 


       FILE SECTION. 
       FD  INPUT-FILE RECORDING MODE F.
       01 INSTRUCTION-LINES  PIC X(LINE-LENGTH).

       WORKING-STORAGE SECTION. 

       01 IDX                PIC 9(4).
       01 ATEND              PIC X.

       LINKAGE SECTION. 

       01 INSTRUCTIONS.
         02 INSTR-DATA OCCURS 128 TIMES.
           05 INSTR-LINE     PIC X(LINE-LENGTH).

       01 INSTR-COUNT        PIC 9(4).

       PROCEDURE DIVISION USING INSTRUCTIONS INSTR-COUNT.


           OPEN INPUT INPUT-FILE.

           MOVE 1 TO IDX.

           DISPLAY "start reading".

           PERFORM UNTIL ATEND = 'Y'
               DISPLAY "read line " IDX
               READ INPUT-FILE INTO INSTR-DATA(IDX)
               AT END
                   MOVE 'Y' TO ATEND 
               NOT AT END
                   ADD 1 TO IDX
               END-READ 
           END-PERFORM.

           CLOSE INPUT-FILE.

           COMPUTE INSTR-COUNT = IDX - 1.

           DISPLAY "loaded " INSTR-COUNT " instructions".

       END PROGRAM LOAD-INSTRUCTIONS.


      * todo: move cursor, check bounds, give end-coordinates, return
      * number