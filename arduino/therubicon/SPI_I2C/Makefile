# ARDUINO_DIR  = /usr/share/arduino
# # ARDUINO_PORT = /dev/ttyACM0
#
# USER_LIB_PATH = ~/sketchbook/libraries
#
# BOARD_TAG    = mega
# BOARD_SUB    = atmega2560
# MONITOR_PORT = /dev/ttyACM*
#
# include /usr/share/arduino/Arduino.mk

all:

# Messaging.o: Messaging.cc Messaging.h
	# gcc std=c++11 -wall -c Messaging.cc Messaging.h

test.o: test.cc Mesaging.h
	gcc std=c++11 -wall -c Main.cc

test: test.o Messaging.h
		gcc std=c++11 -wall test.o Messaging.h -o test
