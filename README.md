# LED74HC595
stm32 C-lib "4-Bit LED Digital Tube Module"
lib uses SPI&TIMER3

7segments LED is:
	                1  A  F 2  3  B
	 _______________|__|__|__|__|__|_____________
	|         |         |    |         |         |
	|    A    |    A    |    |    A    |    A    |
	| F     B | F     B | dp | F     B | F     B |
	|    G    |    G    |    |    G    |    G    |
	| E     C | E     C | dp | E     C | E     C |
	|    D    |    D    |    |    D    |    D    |
	|_________|_________|____|_________|_________|
	                |  |  |  |  |  |
	                E  D dp  C  G  4
	
  The module has two 74HC595 chips, pin mapping:
	     _______
	Q1  |1 *  16|  Vcc                  PINS 1-7, 15   Q0 - Q7   Output Pins
	Q2  |2    15|  Q0                   PIN 8      GND       Ground, Vss
	Q3  |3    14|  DS                   PIN 9      Q7"       Serial Out
	Q4  |4    13|  OE                   PIN 10     MR        Master Reclear, active low
	Q5  |5    12|  ST_CP                PIN 11     SH_CP     Shift register clock pin
	Q6  |6    11|  SH_CP                PIN 12     ST_CP     Storage register clock pin (latch pin)
	Q7  |7    10|  MR                   PIN 13     OE        Output enable, active low
	GND |8_____9|  Q7"                  PIN 14     DS        Serial data input
	                                    PIN 16     Vcc       Positive supply voltage
	           _______
	   LED B -|1 *  16|-5V
	   LED C -|2    15|-LED A
	   LED D -|3    14|-PIN 11
	   LED E -|4    13|-GND
	   LED F -|5    12|-PIN 8
	   LED G -|6    11|-PIN 12 ; 1uF TO GND
	   LED dp-|7    10|-5V
	      GND-|8_____9|-NILL
