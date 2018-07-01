pub mod Messaging{

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

    impl Message{
        pub fn to_msg_vec(&self) -> Vec<u8>{
            match self {
                Message::digital{ref interface,ref pin,ref mode,ref action} => vec![1, 2],
                _ => vec![1, 2]
            }
        }
    }

}
