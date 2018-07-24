extern crate JonesLib;
use JonesLib::Messaging::Messaging::*;

extern crate magnetic;
use std::thread::spawn;
use std::thread;
use magnetic::spsc::spsc_queue;
use magnetic::buffer::dynamic::DynamicBuffer;
use magnetic::{Producer, Consumer};
use std::time::Duration;

extern crate i2cdev;
use i2cdev::core::*;
use i2cdev::linux::{LinuxI2CDevice, LinuxI2CError};

const SLAVE_ADDR: u16 = 0x04;


extern crate spidev;
use spidev::{Spidev, SpidevOptions, SPI_MODE_0};
use spidev::spidevioctl::SpidevTransfer;
use std::io;
use std::mem;



fn create_spi() -> io::Result<Spidev>{
	let mut spidev = Spidev::open("/dev/spidev0.0").unwrap();
    let options = SpidevOptions::new()
                      .bits_per_word(8)
                      .max_speed_hz(5000)
                      .lsb_first(false)
                      .mode(SPI_MODE_0)
                      .build();
    try!(spidev.configure(&options));
    Ok(spidev)
}

fn send_via_i2c(i2cdev : &mut LinuxI2CDevice, msg : &Vec<u8>){
    for byte in msg.iter() {
	(*i2cdev).smbus_write_byte_data(0x04, *byte).unwrap();
	println!("hey");
	thread::sleep(Duration::from_millis(10));
	}
}

fn main() {
    let spidev = create_spi().unwrap();
	let mut i2c_dev = LinuxI2CDevice::new("/dev/i2c-1", SLAVE_ADDR).unwrap();


	let (spi_p, spi_c) = spsc_queue(DynamicBuffer::new(32).unwrap());
	let (i2c_p, i2c_c) = spsc_queue(DynamicBuffer::new(32).unwrap());

	// i2c thread
	let i2c = spawn(move || {
	    loop {
	        let i2c_buf = match i2c_c.pop() {
				Ok(n) => n,
				Err(e) => {println!("badd input {:?}", e); vec![0xFF]}

			};


	        println!("i2c Consumed {:?}", i2c_buf);
	        send_via_i2c(&mut i2c_dev, &i2c_buf);

	    }
	});

	// spi thread
	let spi = spawn(move || {
	    loop {
	        let spi_buf :Vec<u8> = spi_c.pop().unwrap();
	        println!("spi Consumed {:?}", spi_buf);
		    let mut trans = SpidevTransfer::write(&spi_buf[..]);
		    println!("{:?}", spidev.transfer(&mut trans));

	    }
	});
	// main thread
    loop {

	    println!("===== Single transfer =========");

		let mut input = String::new();

		// reads in the line from std
		match io::stdin().read_line(&mut input) {
		    Ok(n) => {
		        println!("{} bytes read", n);
		        println!("{}", input);
		    }
		    Err(error) => println!("error: {}", error),
		}

		let msg : Message = parser(&String::from(input));
		println!("{:?}", msg );


		match msg {
			Message::digital{ref interface, ref pin, ref mode, ref action} => {
				match interface as &str{
					"spi" => spi_p.push(msg.to_msg_vec()).unwrap(), // the unwrap should eventually be handled
					"i2c" => i2c_p.push(msg.to_msg_vec()).unwrap(),
					_ => println!("error")
				}
			},
			Message::servo{ref interface, ref pin, ref action} => {
				match interface as &str{
					"spi" => spi_p.push(msg.to_msg_vec()).unwrap(), // the unwrap should eventually be handled
					"i2c" => i2c_p.push(msg.to_msg_vec()).unwrap(),
					_ => println!("error")
				}
			},
			_ => println!("Error Not analog or lcd is not implemented")
			// all these prints will eventually be switched to error output
		}

	// 	// parses message into arduino readable
	// 	let dev_buf : Vec<u8>= parse_message(&input);
	// 	println!("{:?}",dev_buf );


	//     if buffer.split_whitespace().next().unwrap() == "spi" {
	//     	spi_p.push( buffer);
	//     }else
	//     if buffer.split_whitespace().next().unwrap() == "i2c" {
	//     	i2c_p.push( buffer);
	// 	}

	// i2c.join().unwrap();
	// spi.join().unwrap();


	}
}
