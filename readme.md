#Rusty bot 
This is a robot project that leverages a Raspberry Pi and Arduino. The Raspberry Pi receives commands and feeds them 
to a arduino the aim is to make the command structure very generatic so any arduino and Pi set up can be used, and to 
make this project as plug and play as possible. 

`Feel Free to look through the source code and critic everything. I appreciate the criticism, help and support
`
# Commands # 
These are the commands that are supported by the project. These commands are that control the arduino board.
The purpose of defining the commands as described in this readme is to insure extendablity and replaceablitly. This is currently in draft 2 and so far so good. Only peices are replaced in the grammar rather that be Dcmd or Action. 

## Command structure ##

* Message      ::= { (Interface :  I | i | S | s ) + Command }
* Interface    ::= { SPI or I2c} 
* Command	   ::= { (Digital : D | d + Dcmd) | (Analog : A | a + Acmd) | (Servo : S | s + Scmd) | (DCMotor : m | M + mcmd) }


#### Digtial Read Write ### 

* Dcmd 		   ::= { Pin + ( Read | ( Write + Action) ) }
* Pin          ::= { u8 }
* Read         ::= { R | r}
* Write        ::= { W | w}
* Action   	   ::= { u8 }

#### Analog Read Write ##
* Analog	   ::= { Pin + ( Read | (Write + Action) ) }
* Pin          ::= { u8 + u8 }
* Read         ::= { R | r}
* Write        ::= { W | w}
* Action	   ::= { u8\*}


#### Servo Command Read Write ##
* Scmd		   ::= { Pin + Read | (Write + Degree) }
* Pin		   ::= { u8 }
* Read         ::= { R | r}
* Write        ::= { W | w}
* Degree       ::= { u8 } 




#### DC Motors ##
`this is under construction the commands will be by motor now not by direction `


## Messaging Examples #

* s d 30 w 1
* I d 30 r

# Controller ###
Below is all the details for the controller implementation 
Controller pins:


#####Pin set up for the current hardware controller 
	sw	hardware
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
