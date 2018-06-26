pub mod{


    // add ID to this Like id of the message
    // add enum as to what kind of message can be sent Servo, Digital, Analog,
    // add
    fn parse_message(raw_mess : &String) -> Vec<u8>{
    	// mess
    	let mut message : Vec<u8>= Vec::new();

    	message.push(0x7E);
    	message.push(raw_mess.as_bytes()[1]);
    	for word in raw_mess.split_whitespace(){

    		match word.trim().parse(){
    			Ok(my_num) => message.push(my_num),
    			Err(_) => continue,
    			};
    	}
    	message.push(0xA);
    	message
    }
}
