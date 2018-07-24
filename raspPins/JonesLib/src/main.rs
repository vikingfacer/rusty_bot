
extern crate JonesLib;
use JonesLib::Messaging::Messaging::*;
fn main() {

    let digital_msg1 = Message::digital{interface: String::from("spi"), pin : 30, mode : 'w', action : 1};
    let digital_msg2 = Message::digital{interface: String::from("i2c"), pin : 30, mode : 'w', action : 1};
    let digital_msg3 = Message::digital{interface: String::from("i2c"), pin : 20, mode : 'r', action : 0};

    let d_string_1 = String::from("S D 30 w 1");
    let d_string_2 = String::from("i d 30 w 1\n");
    let m_string_1 = String::from("i m f 255\n");

    print!("{:?}",parser(& m_string_1) );

    for letter in parser(& m_string_1).to_msg_vec(){
        print!("{:?}",letter as char );
    }
    // println!("hi" );
}
