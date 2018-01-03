extern crate rust_pigpio;

use std::thread::sleep;
use rust_pigpio::*;
const PIN1: u32 = 21;
const PIN2: u32 = 13;
//Turns light on and off
fn main() {
  println!("Initialized pigpio. Version: {}", initialize().unwrap());

  blink(PIN1);
  blink(PIN2);

  terminate();
}

fn blink(PIN : u32) {
	  println!("Initialized pigpio. Version: {}", initialize().unwrap());

  set_mode(PIN, OUTPUT).unwrap();
  write(PIN, ON).unwrap();
  sleep(std::time::Duration::from_secs(1));
  write(PIN, OFF).unwrap();
  sleep(std::time::Duration::from_secs(1));
  write(PIN, ON).unwrap();
  sleep(std::time::Duration::from_secs(1));
  write(PIN, OFF).unwrap();
}