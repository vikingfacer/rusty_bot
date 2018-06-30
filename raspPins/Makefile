USER = "pi"
IP = "10.0.0.112"
PASS = "raspberry"
TARGET = --target=armv7-unknown-linux-gnueabihf
TARGET_PATH = "target/armv7-unknown-linux-gnueabihf/debug/raspPins"

all:

pi:
	@cargo build $(TARGET)

upload:
	@sshpass -p $(PASS) @ssh $(TARGET_PATH) $(USER)@$(IP):
