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
        error{message : String},
        digital{interface : String, pin: u8, mode : char, action : u8},
        servo{interface : String, pin : u8, action : u8},
        analog{interface : String, pin : u8, action : u8},
        dcmove{interface : String, direction : char, speed : u8}
    }

    pub fn parser(raw_mess : &String) -> Message{
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
                            _     => return Message::error{message : String::from("interface does not exist")}
            },
            None => return Message::error{message : String::from("interface not specified")}
        };

        let PinType = match parse_this.pop() {
            Some(n) => match n{
                        "d" | "D" => n.as_bytes()[0] as char,
                        "a" | "A" => n.as_bytes()[0] as char,
                        "s" | "S" => n.as_bytes()[0] as char,
                        "l" | "L" => n.as_bytes()[0] as char,
                        "m" | "M" => n.as_bytes()[0] as char,
                            _     => return Message::error{ message : String::from("message type not specified")}
            },
            None => return Message::error{message : String::from("message type not specified")}
        };


        match PinType {
            'l' | 'L' => { let Lcdmsg = parse_this.reverse(); // this more then just reversed
                /* parse_this will bee to be combined into a vector then turned to a vector of u8's */
                    Message::error{message : String::from("LCD not implemented")}
                    },
            'd' | 'D' => {
                    let Pin = match parse_this.pop() {
                            Some( n) => match n.parse::<u8>(){ // this parses the &str into a u8 int
                                  Ok(pin) => pin,
                                  Err(_) => return Message::error{message : String::from("Pin number unparsable")}
                                  }
                            None => return Message::error{message : String::from("Pin number does not exist")}
                            };

                    let Mode = match parse_this.pop() {
                                Some(n) => n.as_bytes()[0] as char,
                                None => return Message::error{message : String::from("Mode does not exist")}
                            };

                    let Action = match parse_this.pop() {
                                Some(n) => match n.parse::<u8>(){ // this parses the &str into a u8
                                                Ok(pin) => pin,
                                                Err(_) => return Message::error{message : String::from("Action not parsable")},
                                                },
                                None => return Message::error{message : String::from("Action does not exist")}
                                };
                    Message::digital{interface : dev, pin : Pin, mode : Mode, action : Action}
                    },
            's' | 'S' => {
                    let Pin = match parse_this.pop() {
                            Some( n) => match n.parse::<u8>(){ // this parses the &str into a u8 int
                                  Ok(pin) => pin,
                                  Err(_) => return Message::error{message : String::from("Pin number unparsable")}
                                  }
                            None => return Message::error{message : String::from("Pin number does not exist")}
                            };
                    let Action = match parse_this.pop() {
                                Some(n) => match n.parse::<u8>(){ // this parses the &str into a u8
                                                Ok(pin) => pin,
                                                Err(_) => return Message::error{message : String::from("Action not parsable")},
                                                },
                                None => return Message::error{message : String::from("Action does not exist")}
                                };
                    Message::servo{interface : dev, pin : Pin, action : Action}
                    },
            'a' | 'A' => {
                    let Pin = match parse_this.pop() {
                            Some(n) => match n.parse::<u8>(){ // this parses the &str into a u8 int
                                  Ok(pin) => pin,
                                  Err(_) => return Message::error{message : String::from("Pin number unparsable")}
                                  }
                            None => return Message::error{message : String::from("Pin number does not exist")}
                            };
                    let Action = match parse_this.pop() {
                                Some(n) => match n.parse::<u8>(){ // this parses the &str into a u8
                                                Ok(pin) => pin,
                                                Err(_) => return Message::error{message : String::from("Action not parsable")},
                                                },
                                None => return Message::error{message : String::from("Action does not exist")}
                                };
                    Message::analog{interface : dev, pin : Pin, action : Action}
                    },

            'm' | 'M' => {
                    let dir = match parse_this.pop() {
                            Some(n) => n.as_bytes()[0] as char,
                            None => return Message::error{message : String::from("dir(ection) does not exist")}
                            };
                    let sp = match parse_this.pop() {
                            Some(n) => match n.parse::<u8>(){ // this parses the &str into a u8 int
                                  Ok(s) => s,
                                  Err(_) => return Message::error{message : String::from("sp(speed) unparsable")}
                                  }
                            None => return Message::error{message : String::from("sp(speed) does not exist")}
                            };
                    Message::dcmove{interface : dev, direction : dir, speed : sp }
                    }
                  _=> Message::error{message : String::from("message type not supported")}
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
                    r_vec.push('\n' as u8);
                },
                Message::servo{ref interface, ref pin, ref action } => {
                    r_vec.push('S' as u8);
                    r_vec.push(*pin);
                    r_vec.push(*action);
                    r_vec.push('\n' as u8);
                },
                Message::dcmove{ref interface, ref direction, ref speed} =>
                {
                    r_vec.push('M' as u8);
                    r_vec.push(*direction as u8);
                    r_vec.push(*speed);
                    r_vec.push('\n' as u8);
                }
                _ => {}
            }
            r_vec
        }
    }

}
