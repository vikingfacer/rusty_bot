extern crate rust_pigpio;
extern crate libc;

use rust_pigpio::*;

use std::fs::File;
use std::io::prelude::*;



const ADDRESS : u32 = 0x04;
const I2_C_DEVICE : & str = "dev/i2c-0"; 

//Turns light on and off
fn main() {

  let f = File::open(I2_C_DEVICE);

  println!("Initialized pigpio. Version: {}", initialize().unwrap());



  terminate();
}
