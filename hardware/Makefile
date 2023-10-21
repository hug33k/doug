MCU=attiny25
AVRDUDEMCU=t25
CC=/usr/bin/avr-gcc
OBJ2HEX=/usr/bin/avr-objcopy
AVRDUDE=/usr/bin/avrdude
TARGET=main
LIBDIR=./lib
SRC=$(TARGET).c $(LIBDIR)/usiTwiSlave.c
#SRC=$(TARGET).c
INC=-I/usr/lib/avr/include/ -I./ -I./lib/
#INC=-I/usr/lib/avr/include/ -I./
CFLAGS=-std=c99 -g -Os -Wall -mcall-prologues -mmcu=$(MCU) $(INC) 
LDFLAGS=-Wl,-gc-sections -Wl,-relax 
AVRDUDECONF=/home/pi/avrdude.conf
AVRDUDEPROG=pi400

all:
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)
	$(OBJ2HEX) -R .eeprom -O ihex $(TARGET) $(TARGET).hex
	rm -f $(TARGET)

install: all
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -C $(AVRDUDECONF) -c $(AVRDUDEPROG) -U flash:w:$(TARGET).hex

fuse:
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -C $(AVRDUDECONF) -c $(AVRDUDEPROG) -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

clean:
	rm -f *.hex *.o *.obj $(TARGET)
