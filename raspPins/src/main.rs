// extern crate i2cdev;

// use std::thread;
// use std::time::Duration;

// use i2cdev::core::*;
// use i2cdev::linux::{LinuxI2CDevice, LinuxI2CError};

// const SLAVE_ADDR: u16 = 0x04;

// // real code should probably not use unwrap()
// fn main() {
//     let mut dev = LinuxI2CDevice::new("/dev/i2c-1", SLAVE_ADDR).unwrap();
//     let mut r_buf :[u8; 6] = [0; 6];
//     // init sequence
//     dev.smbus_write_byte_data(0x04, 1).unwrap();
//     dev.smbus_write_byte_data(0x04, 2).unwrap();
//     thread::sleep(Duration::from_millis(10));

//     dev.read(&mut r_buf).unwrap();
//     println!("{:?}",r_buf );
// }

extern crate spidev;
use spidev::{Spidev, SpidevOptions, SPI_MODE_0};
use spidev::spidevioctl::SpidevTransfer;
use std::io;
use std::mem;

fn parse_message(raw_mess : String) -> Vec<u8>{
	// mess
	let mut message : Vec<u8>= Vec::new();
	
	message.push(0x7E);

	for word in raw_mess.split_whitespace(){

		match word.trim().parse(){
			Ok(my_num) => message.push(my_num),
			Err(_) => continue,
			};
	}
	message.push(0xA);
	message
}

fn main() {
    let mut spidev = Spidev::open("/dev/spidev0.0").unwrap();
    let options = SpidevOptions::new()
                      .bits_per_word(8)
                      .max_speed_hz(5000)
                      .lsb_first(false)
                      .mode(SPI_MODE_0)
                      .build();
    spidev.configure(&options).unwrap();
    loop {
        
	    println!("===== Single transfer =========");

		let mut input = String::new();
		match io::stdin().read_line(&mut input) {
		    Ok(n) => {
		        println!("{} bytes read", n);
		        println!("{}", input);
		    }
		    Err(error) => println!("error: {}", error),
		}
		// for word in input.split(' '){
		// 	println!("split: {:?}", word);
		// }
		// let my_num : u8 = match input.trim().parse(){
		// 	Ok(my_num) => my_num,
		// 	Err(_) => continue,
		// };

		let mut dev_buf : Vec<u8>= parse_message(input);
		println!("{:?}",dev_buf );
		// dev_buf.push(my_num);
		// dev_buf.push(0xA);

		// let mut read_buf= vec!(&dev_buf[..]);

	    let mut trans = SpidevTransfer::write(&dev_buf[..]);


	    println!("{:?}", spidev.transfer(&mut trans));
	}
}




