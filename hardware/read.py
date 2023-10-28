import smbus
import time

# I2C address of the ATTiny25
I2C_ADDRESS = 0x12  # Replace with the correct I2C address
I2C_ADDRESS_2 = 0x24  # Replace with the correct I2C address

I2C_BUS = 1

# Initialize the I2C bus
bus = smbus.SMBus(I2C_BUS)

while (True):
	data = bus.read_byte(I2C_ADDRESS)
	print("Data from " + hex(I2C_ADDRESS) + " = " + hex(data))
	data = bus.read_byte_data(I2C_ADDRESS_2, 1)
	print("Data from " + hex(I2C_ADDRESS_2) + " 1= " + hex(data))
	data = bus.read_byte_data(I2C_ADDRESS_2, 2)
	print("Data from " + hex(I2C_ADDRESS_2) + " 2= " + hex(data))
	data = bus.read_byte_data(I2C_ADDRESS_2, 3)
	print("Data from " + hex(I2C_ADDRESS_2) + " 3= " + hex(data))
	data = bus.read_byte_data(I2C_ADDRESS_2, 4)
	print("Data from " + hex(I2C_ADDRESS_2) + " 4= " + hex(data))
	data = bus.read_byte_data(I2C_ADDRESS_2, 5)
	print("Data from " + hex(I2C_ADDRESS_2) + " 5= " + hex(data))
	data = bus.read_byte_data(I2C_ADDRESS_2, 6)
	print("Data from " + hex(I2C_ADDRESS_2) + " 6= " + hex(data))
	data = bus.read_byte_data(I2C_ADDRESS_2, 7)
	print("Data from " + hex(I2C_ADDRESS_2) + " 7= " + hex(data))
	data = bus.read_byte_data(I2C_ADDRESS_2, 8)
	print("Data from " + hex(I2C_ADDRESS_2) + " 8= " + hex(data))
	time.sleep(1)

# Close the I2C bus
bus.close()

