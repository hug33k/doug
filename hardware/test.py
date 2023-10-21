import smbus
import time

# I2C address of the ATTiny25
I2C_ADDRESS = 0x10  # Replace with the correct I2C address

# Define the commands to send to the ATTiny25
COMMAND_TURN_ON = 0x05
COMMAND_TURN_OFF = 0x00

# I2C bus number on Raspberry Pi (usually 1 for Raspberry Pi 2+)
I2C_BUS = 1

# Initialize the I2C bus
bus = smbus.SMBus(I2C_BUS)

def send_command(command):
    # Send the command to the ATTiny25
    bus.write_byte(I2C_ADDRESS, command)
    time.sleep(0.1)  # Wait for the ATTiny25 to process the command

# Example: Turn on the LED
send_command(COMMAND_TURN_ON)

# Wait for a while
time.sleep(2)

# Example: Turn off the LED
send_command(COMMAND_TURN_OFF)

# Close the I2C bus
bus.close()

