extern crate skeptic;

#[allow(dead_code)] fn readme_0() {
    let ref s = format!("{}", r####"extern crate i2cdev;

use std::thread;
use std::time::Duration;

use i2cdev::core::*;
use i2cdev::linux::{LinuxI2CDevice, LinuxI2CError};

const NUNCHUCK_SLAVE_ADDR: u16 = 0x52;

// real code should probably not use unwrap()
fn i2cfun() -> Result<(), LinuxI2CError> {
    let mut dev = try!(LinuxI2CDevice::new("/dev/i2c-1", NUNCHUCK_SLAVE_ADDR));

    // init sequence
    try!(dev.smbus_write_byte_data(0xF0, 0x55));
    try!(dev.smbus_write_byte_data(0xFB, 0x00));
    thread::sleep(Duration::from_millis(100));

    loop {
        let mut buf: [u8; 6] = [0; 6];
        dev.smbus_write_byte(0x00).unwrap();
        thread::sleep(Duration::from_millis(10));
        dev.read(&mut buf).unwrap();
        println!("Reading: {:?}", buf);
    }
}
"####);
    skeptic::rt::run_test(r#"/Users/jacobmontpetit/RustStuff/rustPi/rusty_bot/rusty_bot/raspPins/target/debug/build/i2cdev-533f9716245657d4/out"#, s);
}


