# Messaging Examples #

* s d 30 w 1
* I d 30 r

# Messaging Commands #
## Grammar ##

* Message      ::= { Interface + Devterm }
* Interface    ::= { I | i | S | s }
* Devterm	     ::= { DevPinAction | LCDAction }

* DevPinAction ::= { DevPin +  PinAction | Degree }
* DevPin       ::= { PinType + Pin }
* PinType      ::= { D | d | A | a | S | s }
* Pin          ::= { u8 int }
* PinAction    ::= { Read | WriteTerm + u8 int}
* Read         ::= {R | r}
* WriteTerm    ::= {W | w + u8}
* Degreee      ::= {u8 int }

* LCDAction    ::= { LCD + LCDmsg }
* LCD          ::= { L | l }
* LCDmsg       ::= { char* }



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
