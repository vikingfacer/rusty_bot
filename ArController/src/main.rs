extern crate serial;
extern crate ws;

use std::env;
use std::io;
use std::time::Duration;
use std::ffi::OsString;
use std::net::{TcpStream, Shutdown};

use std::io::prelude::*;
use serial::prelude::*;
use serial::*;
use std::io::{BufReader, Write, BufWriter};
use std::string::String;

// this program needs a serial device to listen to.
fn create_serial_conn(dev : &OsString ) -> Result<SystemPort>
{
    match serial::open(dev) {
        Ok(mut n) => {n.configure(&PortSettings {
                        baud_rate: BaudRate::Baud115200,
                        char_size: CharSize::Bits8,
                        flow_control: FlowControl::FlowNone,
                        parity: Parity::ParityNone,
                        stop_bits: StopBits::Stop1,});
                        Ok(n)},
        Err(n) => Err(n)
    }
}

fn main() {
    let env_var : Vec<OsString> = env::args_os().skip(1).collect();
    println!("{:?}", env_var);
    if env_var.len() < 2 {
        println!("two few arguments specify Serial Device, and IP connection" );
        return
    }

    let mut port = match create_serial_conn(&env_var[0]) {
        Ok(serial) => serial,
        Err(n) => {println!("{:?}", n);
                    return}
    };

    let reader = BufReader::new(port);


    let ip = String::from(env_var[1].to_str().unwrap());
    let mut stream = match TcpStream::connect( ip + &":8080".to_owned()) {
            Ok(s) => s,
            Err(e) => {println!("error: {:?}", e);
                        return;}
            };

    let mut print = true;
    for line in reader.lines() {
        if line.is_ok() {
           // println!("{:?}", line.unwrap_or("this broke".to_string()));
           let mut transmit_str : String = "I M ".to_owned() + &line.unwrap();

           stream.write(transmit_str.as_bytes());
           print = true;
       }
    }
    stream.shutdown(Shutdown::Both);
    // port.reconfigure
}
