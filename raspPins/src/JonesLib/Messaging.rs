pub mod Messaging{

    // inferface: SPI or I2C first index
    // type: digital Servo  (eventually Analog)
    // Pin: number
    // IF Digital: write or read
    // action: on, off, degree,

    enum Message{
        error,
        digital,
        servo,
        analog
    }

    pub fn parse_message(raw_mess : &String) -> Vec<u8>{
        // example: i d 30 w 1

        // mess
    	let mut message : Vec<u8>= Vec::new();

        // places ~ first
    	message.push(0x7E);
        // places the type {d, s, a, l}
    	// message.push(raw_mess.as_bytes()[1]);

        // println!("{:?}", message);


    	for word in raw_mess.split_whitespace(){
            // println!("{:?}",word);

            if word.as_bytes()[0] >= 48 && word.as_bytes()[0] <= 57{

    		match word.trim().parse(){
    			Ok(my_num) => message.push(my_num),
    			Err(_) => continue,
    			};
            }
            else {
                message.push(word.as_bytes()[0]);
            }
    	}

        //  when these are turned into mesages then well build them from the message

    	message.push(0xA);
    	message
    }

}
