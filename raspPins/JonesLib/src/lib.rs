pub mod Messaging;

#[cfg(test)]
mod tests {
    use Messaging::Messaging::*;

    #[test]
    fn test_it_runs(){
        parse_message(&String::from("s d 30 w 1"));
    }

    #[test]
    fn error_from_parse_message(){
        assert_eq!( Message::error,  parse_message(&String::from("")));
    }

    #[test]
    fn digital_from_parse_message(){
        let digital_msg1 = Message::digital{interface: String::from("spi"), pin : 30, mode : 'w', action : 1};
        let digital_msg2 = Message::digital{interface: String::from("i2c"), pin : 30, mode : 'w', action : 1};
        let digital_msg3 = Message::digital{interface: String::from("i2c"), pin : 20, mode : 'r', action : 0};

        let digital_err  = Message::error;

        assert_eq!(digital_msg1, parse_message(&String::from("s d 30 w 1")));
        assert_eq!(digital_msg1, parse_message(&String::from("S d 30 w 1")));
        assert_eq!(digital_msg1, parse_message(&String::from("S D 30 w 1")));


        assert_eq!(digital_msg2, parse_message(&String::from("i d 30 w 1")));
        assert_eq!(digital_msg2, parse_message(&String::from("I d 30 w 1")));
        assert_eq!(digital_msg3, parse_message(&String::from("I d 20 r 0")));

        assert_eq!(digital_err, parse_message(&String::from("I d 30 1")));

    }

    #[test]
    fn servo_from_parse_message(){
        let servo_msg1 = Message::servo{interface: String::from("spi"), pin : 11, action : 111};
        let servo_msg2 = Message::servo{interface: String::from("i2c"), pin : 11, action : 111};

        assert_eq!(servo_msg1, parse_message(&String::from("s s 11 111")));
        assert_eq!(servo_msg2, parse_message(&String::from("i s 11 111")));

    }

    // #[test]
    // fn implementation(){
    //
    // }

}
