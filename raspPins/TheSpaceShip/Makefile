USER = "pi"
IP = "10.0.0.112"
PASS = "raspberry"
TARGET = --target=armv7-unknown-linux-gnueabihf
TARGET_PATH = "target/armv7-unknown-linux-gnueabihf/debug/rasp*"

all:


# the spaceships recipes
pi:
	@cargo build $(TARGET)

$(TARGET_PATH):
	cargo build $(TARGET)

upload: $(TARGET_PATH)
	sshpass -p $(PASS) ssh $(TARGET_PATH) $(USER)@$(IP):~

# lib recipes

JonesLib:
	# cargo build


# should have 1 run for lib main
# and 2 cleans for the builds
