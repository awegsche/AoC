      *-----------------------------------------------------------------
      *    main procedure ----------------------------------------------
      *-----------------------------------------------------------------
       IDENTIFICATION DIVISION.
       PROGRAM-ID.  DAY01.
       AUTHOR. Andreas Wegscheider. 
       SECURITY. NON-CONFIDENTIAL.

      *-----------------------------------------------------------------
       DATA DIVISION. 

       REPLACE ==MAX-MOVEMENTS== BY ==512==.

       WORKING-STORAGE SECTION. 

      * santa's position
       01 POS-X          PIC S9(4) COMP.
       01 POS-Y          PIC S9(4) COMP.
       01 DIST           PIC S9(4) COMP.
      * the movement list
       01 MOVEMENTS.
         02 MOV-DATA OCCURS MAX-MOVEMENTS TIMES.
           05 DIRECTION  PIC X.
           05 DISTANCE   PIC 9(5).
       01 MOV-PTR        PIC S9(4) COMP.
       01 MOV-CNT        PIC S9(4) COMP.

       PROCEDURE DIVISION .

      *    read movements from file
           CALL "READ-MOVEMENTS" USING MOVEMENTS, MOV-CNT.
           IF RETURN-CODE NOT = 0 THEN
               DISPLAY "error reading movements"
               GOBACK 
           END-IF.

           DISPLAY "after read-movements, have " MOV-CNT " movements".
           DISPLAY "movement(1) = " MOV-DATA(1).
              
      *    move santa according to instructions
           CALL "MOVE-SANTA" USING MOVEMENTS MOV-CNT POS-X POS-Y.
           DISPLAY "got coords from move-santa: " POS-X " " POS-Y.
           CALL "CALC-DISTANCE" USING POS-X POS-Y DIST.
           DISPLAY "santa distance: " DIST.

      *    move santa according to updated instructions
           CALL "FIND-BUNNY" USING MOVEMENTS MOV-CNT POS-X POS-Y.
           DISPLAY "got coords from move-santa: " POS-X " " POS-Y.
           CALL "CALC-DISTANCE" USING POS-X POS-Y DIST.
           DISPLAY "santa distance: " DIST.

           GOBACK.

      *-----------------------------------------------------------------
      *    calc distance -----------------------------------------------
      *-----------------------------------------------------------------
       ID DIVISION. 
       PROGRAM-ID. CALC-DISTANCE.

       DATA DIVISION. 
       LINKAGE SECTION. 
       01 POS-X          PIC S9(4) COMP.
       01 POS-Y          PIC S9(4) COMP.
       01 DIST           PIC S9(4) COMP.

       PROCEDURE DIVISION USING POS-X POS-Y DIST.

           COMPUTE DIST = FUNCTION ABS(POS-X) + FUNCTION ABS(POS-Y).

       END PROGRAM CALC-DISTANCE.

      *-----------------------------------------------------------------
      *    read movements ----------------------------------------------
      *-----------------------------------------------------------------
       ID DIVISION. 
       PROGRAM-ID. READ-MOVEMENTS.

      *-----------------------------------------------------------------
       ENVIRONMENT DIVISION. 
       INPUT-OUTPUT SECTION. 
       FILE-CONTROL. 
           SELECT INPUT-FILE ASSIGN TO "input01.txt"
              ORGANIZATION IS SEQUENTIAL.
       
      *-----------------------------------------------------------------
       DATA DIVISION. 


       FILE SECTION. 

       REPLACE ==BUFSIZE== BY ==1024==.

       FD  INPUT-FILE RECORDING MODE F.
      *    record varying 1 to 512 depending on bytes-read.
       01 FILE-BUFFER    PIC X(BUFSIZE).

       WORKING-STORAGE SECTION. 

       REPLACE ==MAX-MOVEMENTS== BY ==512==.

      * file reading facilities
       01 TMP-DIGIT      PIC 9.

       01 FILE-PTR       PIC S9(4) COMP.
       01 BYTES-READ     PIC 9(3).
       01 MOV-PTR        PIC S9(4) COMP.


       LINKAGE SECTION. 
      * the movement list
       01 MOVEMENTS.
         02 MOV-DATA OCCURS MAX-MOVEMENTS TIMES.
           05 DIRECTION  PIC X.
           05 DISTANCE   PIC 9(5).
       01 MOV-CNT        PIC S9(4) COMP.

       PROCEDURE DIVISION USING MOVEMENTS MOV-CNT.

      *     read file into memory --------------------------------------
           MOVE LOW-VALUES TO FILE-BUFFER.
           OPEN INPUT INPUT-FILE.
           READ INPUT-FILE NEXT RECORD.
           CLOSE INPUT-FILE.

           PERFORM VARYING BYTES-READ FROM 1 BY 1
              UNTIL BYTES-READ > LENGTH OF FILE-BUFFER 
               IF FILE-BUFFER(BYTES-READ:1) = LOW-VALUES 
                   EXIT PERFORM 
               END-IF
           END-PERFORM.

           SUBTRACT 1 FROM BYTES-READ.

           IF BYTES-READ = LENGTH OF FILE-BUFFER THEN
               DISPLAY "ERROR: input buffer exhausted. "
                       "This program has insufficient working storage "
                       "to handle the specified input file."

               MOVE -1 TO RETURN-CODE
               EXIT PROGRAM 
           END-IF.


           DISPLAY "'" FILE-BUFFER "'".

      *     parse movements --------------------------------------------
           MOVE 1 TO MOV-PTR.
           MOVE 1 TO FILE-PTR.
           PERFORM UNTIL FILE-PTR > BYTES-READ 
               MOVE FILE-BUFFER(FILE-PTR:1) TO DIRECTION(MOV-PTR)

               ADD 1 TO FILE-PTR
               PERFORM UNTIL FILE-PTR > BYTES-READ
                  OR FILE-BUFFER(FILE-PTR:1) = ','
      *          move file-buffer (file-ptr :1)
      *           to distance(mov-ptr+1)
                   MOVE FILE-BUFFER(FILE-PTR:1) TO TMP-DIGIT 
                   COMPUTE DISTANCE(MOV-PTR) = DISTANCE
                      (MOV-PTR) * 10
                      + TMP-DIGIT 
                   ADD 1 TO FILE-PTR
                 
               END-PERFORM
               ADD 1 TO FILE-PTR

      *       skip whitespace
               PERFORM UNTIL FILE-BUFFER(FILE-PTR:1) NOT = ' '
                   ADD 1 TO FILE-PTR 
               END-PERFORM

               ADD 1 TO MOV-PTR 
               IF MOV-PTR = MAX-MOVEMENTS THEN
                   DISPLAY "ERROR: too many movements for this program."
                   MOVE -1 TO RETURN-CODE 
                   EXIT PROGRAM
           END-PERFORM.

           COMPUTE MOV-CNT = MOV-PTR - 1.
           DISPLAY "read " MOV-CNT "movements".

       END PROGRAM READ-MOVEMENTS.

      *-----------------------------------------------------------------
      *    move santa --------------------------------------------------
      *-----------------------------------------------------------------
       ID DIVISION. 
       PROGRAM-ID. MOVE-SANTA.

      *-----------------------------------------------------------------
       DATA DIVISION. 

       WORKING-STORAGE SECTION. 

       01 MOV-PTR        PIC S9(4) COMP.
       01 FACING         PIC S9.
       
       01 DELTA-POS-X    PIC S9(4) COMP.
       01 DELTA-POS-Y    PIC S9(4) COMP.

       LINKAGE SECTION. 
      * the movement list
       01 MOVEMENTS.
         02 MOV-DATA OCCURS MAX-MOVEMENTS TIMES.
           05 DIRECTION  PIC X.
           05 DISTANCE   PIC 9(5).
       01 MOV-CNT        PIC S9(4) COMP.

       01 POS-X          PIC S9(4) COMP.
       01 POS-Y          PIC S9(4) COMP.

       PROCEDURE DIVISION USING MOVEMENTS MOV-CNT POS-X POS-Y.

           MOVE 0 TO FACING.

           PERFORM VARYING MOV-PTR FROM 1 BY 1 UNTIL MOV-PTR > MOV-CNT

      *        DISPLAY MOV-DATA(MOV-PTR)

               EVALUATE FACING 
               WHEN 0 
                   MOVE 1 TO DELTA-POS-X 
                   MOVE 0 TO DELTA-POS-Y 
               WHEN 1
                   MOVE 0 TO DELTA-POS-X 
                   MOVE 1 TO DELTA-POS-Y 
               WHEN 2
                   MOVE -1 TO DELTA-POS-X 
                   MOVE 0 TO DELTA-POS-Y 
               WHEN 3
                   MOVE 0 TO DELTA-POS-X 
                   MOVE -1 TO DELTA-POS-Y 
               END-EVALUATE

               EVALUATE DIRECTION(MOV-PTR)
               WHEN 'R'
                   COMPUTE POS-X = POS-X + DISTANCE(MOV-PTR)
                      * DELTA-POS-X 
                   COMPUTE POS-Y = POS-Y + DISTANCE(MOV-PTR)
                      * DELTA-POS-Y 
                   ADD 1 TO FACING
               WHEN 'L'
                   COMPUTE POS-X = POS-X - DISTANCE(MOV-PTR)
                      * DELTA-POS-X 
                   COMPUTE POS-Y = POS-Y - DISTANCE(MOV-PTR)
                      * DELTA-POS-Y 
                   SUBTRACT 1 FROM FACING
               END-EVALUATE

               IF FACING = -1 THEN
                   MOVE 3 TO FACING
               END-IF 
               IF FACING = 4 THEN
                   MOVE 0 TO FACING
               END-IF 

               DISPLAY "("
                       POS-X
                       ", "
                       POS-Y
                       ") "
                       FACING
                       " "
                       MOV-DATA(MOV-PTR)
              
           END-PERFORM.

           DISPLAY "santa's position: (" POS-X ", " POS-Y ")".

       END PROGRAM MOVE-SANTA.

      *-----------------------------------------------------------------
      *    move santa --------------------------------------------------
      *-----------------------------------------------------------------
       ID DIVISION. 
       PROGRAM-ID. FIND-BUNNY.

      *-----------------------------------------------------------------
       DATA DIVISION. 

       WORKING-STORAGE SECTION. 

       01 MOV-PTR        PIC S9(4) COMP.
       01 FACING         PIC S9.
       
       01 DELTA-POS-X    PIC S9(4) COMP.
       01 DELTA-POS-Y    PIC S9(4) COMP.

       01 IDX            PIC 9(4).

       REPLACE ==NLINES== BY ==1024==.

       01 VLINES OCCURS NLINES TIMES.
         05 HLINES OCCURS NLINES TIMES.
           10 ADDRES     PIC X VALUE '0'.

       LINKAGE SECTION. 
      * the movement list
       REPLACE ==MAX-MOVEMENTS== BY ==512==.
       01 MOVEMENTS.
         02 MOV-DATA OCCURS MAX-MOVEMENTS TIMES.
           05 DIRECTION  PIC X.
           05 DISTANCE   PIC 9(5).
       01 MOV-CNT        PIC S9(4) COMP.
       

       01 POS-X          PIC S9(4) COMP.
       01 POS-Y          PIC S9(4) COMP.

       PROCEDURE DIVISION USING MOVEMENTS MOV-CNT POS-X POS-Y.

           MOVE 0 TO FACING.
           MOVE 0 TO POS-X.
           MOVE 0 TO POS-Y.

           PERFORM VARYING MOV-PTR FROM 1 BY 1 UNTIL MOV-PTR > MOV-CNT

      *        DISPLAY MOV-DATA(MOV-PTR)

               EVALUATE FACING 
               WHEN 0 
                   MOVE 1 TO DELTA-POS-X 
                   MOVE 0 TO DELTA-POS-Y 
               WHEN 1
                   MOVE 0 TO DELTA-POS-X 
                   MOVE 1 TO DELTA-POS-Y 
               WHEN 2
                   MOVE -1 TO DELTA-POS-X 
                   MOVE 0 TO DELTA-POS-Y 
               WHEN 3
                   MOVE 0 TO DELTA-POS-X 
                   MOVE -1 TO DELTA-POS-Y 
               END-EVALUATE

               EVALUATE DIRECTION(MOV-PTR)
               WHEN 'R'
                   PERFORM VARYING IDX FROM 1 BY 1
                      UNTIL IDX > DISTANCE(MOV-PTR)
                       COMPUTE POS-X = POS-X + DELTA-POS-X 
                       COMPUTE POS-Y = POS-Y + DELTA-POS-Y 
                       IF HLINES(POS-Y + 512, POS-X + 512) = '1' THEN
                           GO TO FOUND-BUNNY 
                       END-IF 
                       MOVE '1' TO HLINES(POS-Y + 512, POS-X + 512)
                   END-PERFORM
                   ADD 1 TO FACING
               WHEN 'L'
                   PERFORM VARYING IDX FROM 1 BY 1
                      UNTIL IDX > DISTANCE(MOV-PTR)
                       COMPUTE POS-X = POS-X - DELTA-POS-X 
                       COMPUTE POS-Y = POS-Y - DELTA-POS-Y 
                       IF HLINES(POS-Y + 512, POS-X + 512) = '1' THEN
                           GO TO FOUND-BUNNY 
                       END-IF 
                       MOVE '1' TO HLINES(POS-Y + 512, POS-X + 512)
                   END-PERFORM
                   SUBTRACT 1 FROM FACING
               END-EVALUATE

               IF FACING = -1 THEN
                   MOVE 3 TO FACING
               END-IF 
               IF FACING = 4 THEN
                   MOVE 0 TO FACING
               END-IF 

               DISPLAY "("
                       POS-X
                       ", "
                       POS-Y
                       ") "
                       FACING
                       " "
                       MOV-DATA(MOV-PTR)

              
           END-PERFORM.

       FOUND-BUNNY.
           DISPLAY "found bunny at: (" POS-X ", " POS-Y ")".


       END PROGRAM FIND-BUNNY.