extern crate i2cdev;

use std::thread;
use std::time::Duration;

use i2cdev::core::*;
use i2cdev::linux::{LinuxI2CDevice, LinuxI2CError};

const SLAVE_ADDR: u16 = 0x04;

// real code should probably not use unwrap()
fn main() {
    let mut dev = LinuxI2CDevice::new("/dev/i2c-1", SLAVE_ADDR).unwrap();
    let mut r_buf :[u8; 6] = [0; 6];
    // init sequence
    dev.smbus_write_byte_data(0x04, 1).unwrap();
    dev.smbus_write_byte_data(0x04, 2).unwrap();
    thread::sleep(Duration::from_millis(10));

    dev.read(&mut r_buf).unwrap();
    println!("{:?}",r_buf );
}