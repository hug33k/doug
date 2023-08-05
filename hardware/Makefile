MCU=attiny25
AVRDUDEMCU=t25
CC=/usr/bin/avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=$(MCU)
OBJ2HEX=/usr/bin/avr-objcopy
AVRDUDE=/usr/local/bin/avrdude
TARGET=blink
AVRDUDECONF=/home/pi/avrdude.conf
AVRDUDEPROG=pi400

all:
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)
	$(OBJ2HEX) -R .eeprom -O ihex $(TARGET) $(TARGET).hex
	rm -f $(TARGET)

install: all
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -C $(AVRDUDECONF) -c $(AVRDUDEPROG) -U flash:w:$(TARGET).hex

fuse:
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -C $(AVRDUDECONF) -c $(AVRDUDEPROG) -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

clean:
	rm -f *.hex *.o *.obj
