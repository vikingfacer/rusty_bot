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

