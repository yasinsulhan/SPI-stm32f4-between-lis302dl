# SPI-stm32f4-between-lis302dl

Serial Peripheral Interface (SPI) is an interface bus commonly used to send data between microcontrollers and small peripherals such as shift registers, sensors, and SD cards. It uses separate clock and data lines, along with a select line to choose the device. There are four pins between master and slave for SPI. Three wire is possible also. The master device (STM32f407 VG) having SCK (serial data clock), MISO (Master-In Serial-Out), MOSI (Master-Out Serial-In), and CS (Chip-Select) is connected slave (lis302dl accelorator sensor) having CS, SDI (Serial Data Input), SCL (SPI CK Line) and SDO (Serial Data Output). The connection is shown below. The transmitter that is stm32f initiates a transfer, generates clock signals and terminates a transfer. The slave that is lis302dl addressed by the master. The LIS302DL SPI is a bus slave. The SPI allows to write and read the registers of the device. 4 wires which are CS, SPC, SPDI and SPDO interact the stm32f407.

![1](https://github.com/yasinsulhan/SPI-stm32f4-between-lis302dl/assets/109728194/4c303381-7731-417b-839f-a7ffabc29b81)

In this project, x, y and z variables that indicate position of lis302dl sensor are defined. And these variables change, when the lis302dl is rotated to different positions. Note that, STM32F407VG has the sensor.

Read & protocol provide to adjusting lis302dl sensor by using SPI comminication protocol. SPI is provided via stm32f407 also. As explained in the protocol, **CS is the Chip Select and it is controlled by the SPI master. It goes low at the start of the transmission and goes back high at the end. SPC is the Serial Port Clock and it is controlled by the SPI master. It is stopped high when CS is high (no transmission). SDI and SDO are respectively the Serial Port Data Input and Output. Those lines are driven at the falling edge of SPC and should be captured at the rising edge of SPC.**

![2](https://github.com/yasinsulhan/SPI-stm32f4-between-lis302dl/assets/109728194/1df231ad-710e-4eb2-8736-b337f588239d)

When the data read, CS pin which is E3 must be reset. The address is stored then incremented. And then the data is stored. CS pin is set. When the data writed, the same thing in the reading method must be done. These are just done with write mode. These process are commanded with 16 clock pulses. Also, notice that the needed register addresses must be kept to make process. These are **Out_X**, **Out_Y**, **Out_Z** and ** **Ctrl_Reg1**. X, and Z address keep the direction of movement. Ctrl_Reg1 adjust the lis302dl settings. On the other hand, STM32f registers must be adjusted according to referance manuel. Notice that NSS (Slave Select) pin **selects a slave device. This pin acts as a ‘chip select’ to let the SPI master communicate with slaves individually and to avoid contention on the data lines.** This can be ignored via SSM and SSI bit. Thus, **NSS pin see a low level and become slaves when they are configured in NSS hardware mode. **the master device transmits data to a slave device via the MOSI pin, the slave device responds via the MISO pin.** That is why called full dublex comminication lets datas goes from input to output or output to input with the same clock signal that is produced by the master device via SCK pin as shown below.

![3](https://github.com/yasinsulhan/SPI-stm32f4-between-lis302dl/assets/109728194/73493a58-e8a7-4492-943e-0ef4c8e8f053)

According to datasheet of stm32f407, A5, A6, A7 pin is set as alternate function. E3 pin is set as output. 

![ekle](https://github.com/yasinsulhan/SPI-stm32f4-between-lis302dl/assets/109728194/ec26b860-186c-4f3b-9e0f-9d8c20c6dbf7)

There are four combination of timing relationships presents flexibility of SPI by using CPOL and CPHA bits in the SPI_CR1 register. The CPOL (clock polarity) bit controls the steady state value of the clock when no data is being transferred. This bit affects both master and slave modes. If CPOL is reset, the SCK pin has a low-level idle state. If CPOL is set, the SCK pin has a high-level idle state. If the CPHA (clock phase) bit is set, the second edge on the SCK pin (falling edge if the CPOL bit is reset, rising edge if the CPOL bit is set) is the MSBit capture strobe. The combination and what expected shown below. According to my experience CPOL=0 and CPHA=1 makes delaying the reaction time of accelorometer sensor.

![4](https://github.com/yasinsulhan/SPI-stm32f4-between-lis302dl/assets/109728194/aeb213cc-98e5-426b-a44e-1dc1c8057e5a)

The transmit is began when a byte is written in the Tx Buffer. The data byte is parallel-loaded into the shift register (from the internal bus) during the first bit transmission and then shifted out serially to the MOSI pin MSB first or LSB first. **The TXE flag is set on the transfer of data from the Tx Buffer to the  shift register.** This can be done via while loop during writing or reading period. When data transfer is completedthe data in the shift register is transferred to the RX Buffer and the RXNE flag is set. According to type of buffer, Either transmiting and receiving are stored in data register (DR). At the last sampling clock edge the RXNE bit is set, a copy of the data byte received in the shift register is moved to the Rx buffer. When the SPI_DR register is read, the SPI peripheral returns this buffered value. **A continuous transmit stream can be maintained if the next data to be transmitted is put in the Tx buffer once the transmission is started**. In our project, this is repeated more than one time. Note that TXE flag should be ‘1 before any attempt to write the Tx buffer is made.

## Procedure of SPI
1- SCK rate is set in the BR[2:0] bits in the SPI_CR1 register.

2- DFF bit is set as 8 bit.

3- The frame format is set as MSB (starts from first bit).

4- Software slave menagement in CR1 register is set as explained before.

5- SPI is enabled to avoid any confuse related the data sent from rising edge before.

![gif](https://github.com/yasinsulhan/SPI-stm32f4-between-lis302dl/assets/109728194/e9949020-ce77-4670-be1d-3f7b17fe86fc)
