extern crate serial;
extern crate ws;

use std::env;
use std::io;
use std::time::Duration;
use std::ffi::OsString;
use std::net::TcpStream;

use std::io::prelude::*;
use serial::prelude::*;
use serial::*;
use std::io::{BufReader, Write};

// this program needs a serial device to listen to.
fn create_serialA_conn(dev : &OsString ) -> Result<SystemPort>
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

    let mut port = create_serialA_conn(&env_var[0]).unwrap();
    let reader = BufReader::new(port);
    let mut stream = match TcpStream::connect("127.0.0.1:8080") {
            Ok(s) => s,
            Err(e) => {println!("error: {:?}", e);
                        return;}
            };

    for line in reader.lines() {
        if line.is_ok() {
           // println!("{:?}",  &line.unwrap());
           stream.write(line.unwrap().as_bytes());
        }
        }

    // port.reconfigure
}
