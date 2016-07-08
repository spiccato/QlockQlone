# QlockQlone
Word Clock Prototype using Arduino

Hardware Project using ICs and Arduino Uno to replicate a Qlock (http://www.qlocktwo.com/info.php?lang=en)

Essentially, this clock works by lighting up words(i.e. hours, minutes) within a matrix of letters to tell the time.
The minutes are stated in 5 minute intervals (i.e. FIVE, TEN, FIFTEEN, etc). There is a small LED light at each corner of the clock frame to count the minutes between each of these intervals (i.e. if 3 lights are on, add 3 minutes to the time shown on the face)
As well, there are keywords "PAST" and "TO" that also light up

Parts Used:
- Single Flipflop - Part #74LVC1G74DC,125
- 4x2:1 Mux - Part # SN74HC157N 
- 4 bit Bidirectional Shift Register - Part # SN54S194
- 12 bit Shift Register - Part # TLC6C5912QDWRQ1

    ** IC was not through-hole, needed to solder on wires
 


