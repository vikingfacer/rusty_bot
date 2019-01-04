extern crate serial;
extern crate ws;

use std::env;
use std::ffi::OsString;
use std::io;
use std::net::{Shutdown, TcpStream};
use std::time::Duration;

use serial::prelude::*;
use serial::*;

use std::io::prelude::*;
use std::io::{BufReader, BufWriter, Write};

// this program needs a serial device to listen to.
fn create_serial_conn(dev: &OsString) -> Result<SystemPort> {
    match serial::open(dev) {
        Ok(mut n) => {
            n.configure(&PortSettings {
                baud_rate: BaudRate::Baud115200,
                char_size: CharSize::Bits8,
                flow_control: FlowControl::FlowNone,
                parity: Parity::ParityNone,
                stop_bits: StopBits::Stop1,
            });
            Ok(n)
        }
        Err(n) => Err(n),
    }
}

fn main() {
    let env_var: Vec<OsString> = env::args_os().skip(1).collect();
    println!("{:?}", env_var);
    if env_var.len() < 2 {
        println!("two few arguments specify Serial Device, and IP connection" );
        return
    }

    let mut port = match create_serial_conn(&env_var[0]) {
        Ok(serial) => serial,
        Err(n) => {
            println!("{:?}", n);
            return;
        }
    };

    let reader = BufReader::new(port);

    let mut stream = match TcpStream::connect("10.0.0.112:8080") {
        Ok(s) => s,
        Err(e) => {
            println!("error: {:?} \n NO CONNECTION", e);
            return;
        }
    };
    let mut last_line = String::new();
    for line in reader.lines() {
        if line.is_ok() {
            // println!("{:?}", line.unwrap_or("this broke".to_string()));
            // let mut transmit_str : String = "I M ".to_owned() + &line.unwrap();
            match line {
                Ok(buf) =>
                    if buf != last_line{
                        stream.write(buf.as_bytes());
                        last_line = buf;
                    },
                Err(_) => continue
            };
        }
    }
    stream.shutdown(Shutdown::Both);
    // port.reconfigure
}
