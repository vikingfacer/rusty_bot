# Messaging Examples #

* s d 30 w 1
* I d 30 r

# Messaging Commands #
## Command structure ##

* Message      ::= { (Interface :  I | i | S | s ) + Command }
* Interface    ::= { SPI or I2c} 
* Command	   ::= { (Digital : D | d + Dcmd) | (Analog : A | a + Acmd) | (Servo : S | s + Scmd) }


## Digtial Read Write ## 

* Dcmd 		   ::= { Pin + ( Read | ( Write + Action) ) }
* Pin          ::= { u8 }
* Read         ::= { R | r}
* Write        ::= { W | w}
* Action   	   ::= { u8 }

## Analog Read Write ##
* Analog	   ::= { Pin + }


## Servo Command Read Write ##
* Degree       ::= { u8 }
* PinType      ::= { D | d | A | a | S | s }


## DC Motors ##
this is under construction 


Controller pins:
sw	hdw
	A4 : Right Y axis joystick
	A5 : Right X axis joystick
	A6 : Left  X axis joystick
	A7 : Left  Y axis joystick

	
	D12: Left  Button read
	D10: Right Button read


	D6 : Right green  LED 
	D5 : Left  green  LED
	D4 : Right yellow LED
	D2 : red   LED 
