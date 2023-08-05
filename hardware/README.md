# Hardware for Doug

## Pinout

| ATTiny25 | Pin | RPi 400 | Pin |
|----------|-----|---------|-----|
| Reset    |   1 | GPIO12  |  32 |
| -------- | --- | ------- | --- |
| LED      |   3 | GND     |   6 |
| GNC      |   4 | GND     |   6 |
| MOSI     |   5 | GPIO23  |  16 |
| MISO     |   6 | GPIO18  |  12 |
| SCK      |   7 | GPIO24  |  18 |
| VCC      |   8 | 3.3V    |   1 |
|----------|-----|---------|-----|

## Avrdude config

````conf
programmer
    id = "pi400";
    desc = "pi400";
    type = "linuxgpio";
    reset = 12;
    sck = 24;
    mosi = 23;
    miso = 18;
;
````

## Compilation & Installation

````sh
$> make clean
$> make
$> make install
````

## TODO

- [ ] Script to flash ATTiny25
- [ ] Generate random ID for each ATTiny25 (if they don't already have one / update system to avoid clear ID?)
- [ ] Implement switch support (Trigger LED when switch pressed, for test)
- [ ] Connect to RPi via I2C (I2C address based on ID)
