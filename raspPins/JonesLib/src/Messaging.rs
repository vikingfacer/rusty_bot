pub mod Messaging{

    use std::str;
    // inferface: SPI or I2C first index
    // type: digital Servo  (eventually Analog)
    // Pin: number
    // IF Digital: write or read
    // action: on, off, degree,

    //  need to add lcd
    #[derive(Debug, PartialEq)]
    pub enum Message{
        error,
        digital{interface : String, pin: u8, mode : char, action : u8},
        servo{interface : String, pin : u8, action : u8},
        analog{interface : String, pin : u8, action : u8}
    }

    pub fn parse_message(raw_mess : &String) -> Message{
        // example: i d 30 w 1

        // mess
    	let mut msg : Vec<u8>= Vec::new(); // this will be replaced with the struct

    	for word in raw_mess.split_whitespace(){
            // println!("{:?}",word);

            // if the value of the byte is numeric then process it to a number
            // else push it on as ascii value
            if word.as_bytes()[0] >= 48 && word.as_bytes()[0] <= 57{

    		match word.trim().parse(){
    			Ok(my_num) => msg.push(my_num),
    			Err(_) => continue,
    			};
            }
            else {
                msg.push(word.as_bytes()[0]);
            }
    	}

        if msg.len() < 4{
            return Message::error;
        }

        let interface_v = match msg[0] as char{
            'i' | 'I' => String::from("i2c"),
            's' | 'S' => String::from("spi"),
            _ => {return Message::error;}
        };

        // this if statement is essentially the return
        if msg.len() == 4 {
            match msg[1] as char{
                's' | 'S' => Message::servo{interface : interface_v, pin : msg[2], action : msg[3]},
                'a' | 'A' => Message::analog{interface : interface_v, pin : msg[2], action : msg[3]},
                _ => Message::error
            }

        }else{
            // the catch all is a enough prevention rightnow from malformed messages
            match msg[1] as char{
                'd' | 'D' => Message::digital{interface : interface_v, pin : msg[2], mode : msg[3] as char, action : msg[4]},
                _ => Message::error
            }
        }
    }

    pub fn new_parser(raw_mess : &String) -> Message{
        let mut parse_this : Vec<&str>  = raw_mess.split_whitespace().collect();
        parse_this.reverse();
        // this turns i d 30 w 1
        //  to 1 w 30 d i
        //  this is easier to process like an AST

        // Message      ::= { Interface + Devterm }
        // Interface    ::= { I | i | S | s }
        let dev = match parse_this.pop() {
            Some(n) => match n{
                        "s" | "S" => "spi".to_string(),
                        "i" | "I" => "i2c".to_string(),
                            _     => return Message::error
            },
            None => return Message::error
        };

        let PinType = match parse_this.pop() {
            Some(n) => match n{
                        "d" | "D" => n.as_bytes()[0] as char,
                        "a" | "A" => n.as_bytes()[0] as char,
                        "s" | "S" => n.as_bytes()[0] as char,
                        "l" | "L" => n.as_bytes()[0] as char,
                            _     => return Message::error
            },
            None => return Message::error
        };


        match PinType {
          'l' | 'L' => { let Lcdmsg = parse_this.reverse(); // this more then just reversed
            /* parse_this will bee to be combined into a vector then turned to a vector of u8's */
                    Message::error
                    },
         'd' | 'D' => {
                    let Pin = match parse_this.pop() {
                            Some( n) => match n.parse::<u8>(){ // this parses the &str into a u8 int
                                  Ok(pin) => pin,
                                  Err(_) => return Message::error
                                  }
                            None => return Message::error
                            };

                    let Mode = match parse_this.pop() {
                                Some(n) => n.as_bytes()[0] as char,
                                None => return Message::error
                                };

                    let Action = match parse_this.pop() {
                                Some(n) => match n.parse::<u8>(){ // this parses the &str into a u8
                                                Ok(pin) => pin,
                                                Err(_) => return Message::error,
                                                },
                                None => return Message::error
                                };
                    Message::digital{interface : dev, pin : Pin, mode : Mode, action : Action}
                    },
          's' | 'S' => {
                    let Pin = match parse_this.pop() {
                            Some( n) => match n.parse::<u8>(){ // this parses the &str into a u8 int
                                  Ok(pin) => pin,
                                  Err(_) => return Message::error
                                  }
                            None => return Message::error
                            };
                    let Action = match parse_this.pop() {
                                Some(n) => match n.parse::<u8>(){ // this parses the &str into a u8
                                                Ok(pin) => pin,
                                                Err(_) => return Message::error,
                                                },
                                None => return Message::error
                                };
                    Message::servo{interface : dev, pin : Pin, action : Action}
                    },
          'a' | 'A' => {
                    let Pin = match parse_this.pop() {
                            Some( n) => match n.parse::<u8>(){ // this parses the &str into a u8 int
                                  Ok(pin) => pin,
                                  Err(_) => return Message::error
                                  }
                            None => return Message::error
                            };
                    let Action = match parse_this.pop() {
                                Some(n) => match n.parse::<u8>(){ // this parses the &str into a u8
                                                Ok(pin) => pin,
                                                Err(_) => return Message::error,
                                                },
                                None => return Message::error
                                };                    
                    Message::analog{interface : dev, pin : Pin, action : Action}
                    },
                  _=> Message::error
      }
        // parse_this
    }

    impl Message{
        // Message is created from ~ (0x7E),<Type>, <Pin>, <Mode if digital>, <Action>

        pub fn to_msg_vec(&self) -> Vec<u8>{

            let mut r_vec : Vec<u8> = vec![0x7E];

            match self {
                Message::digital{ref interface,ref pin,ref mode,ref action} =>{
                    r_vec.push('D' as u8);
                    r_vec.push(*pin);
                    r_vec.push(*mode as u8);
                    r_vec.push(*action);
                },
                _ => {}
            }
            r_vec
        }
    }

}
