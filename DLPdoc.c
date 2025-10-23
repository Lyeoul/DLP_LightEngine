//User I2C Command set (I2C bus speed：100 kbit/s；I2C commands conform to the Philips I2C specification)
//Command format:
//Slave device address:0x1B [0x37 (Read) / 0x36 (Write)]
//1st Byte = Command
//2nd Byte = Data [0]
//3rd Byte = Data [1]


// (1) LED Current Setting
Command: 0x54
Data [0] Reserved ( set to 0x00)
Data [1] Reserved ( set to 0x00)
Data [2] LED current parameter (LSByte)
Data [3] LED current parameter (MSByte)
Data [4] Reserved ( set to 0x00)
Data [5] Reserved ( set to 0x00)

//LED Current Formula=[(Bit value+1)/1024] x 6 For Bit value=1 to 1023
//The step of current setting is 5.8593mA.\Example as below,
//If the UV LED current is 5000mA then the setting value is 0x354 (5000/5.8593 = 852 = 0x354).

Command: 0x54 // Write LED current
Data[0] = 0x00 //
Data[1] = 0x00 //
Data[2] = 0x54 // LED current parameter (LSB)
Data[3] = 0x03 // LED current parameter (MSB)
Data[4] = 0x00 //
Data[5] = 0x00 //

// (2) LED Enable / Disable
Command : 0x52 // Write LED Enable / Disable
Data[0] = 0x02 // LED enabled
Command : 0x52 // Write LED Enable / Disable
Data[0] = 0x00 // LED disabled

//Note ： Please make sure the video signal presented, otherwise the LED will be turn off automatically.

// (3) Read LED Junction Temperature
Step 1: write command
    Command: 0xD6
Step 2: read data
    Data [0] LED Junction Temperature (LSByte)
    Data [1] LED Junction Temperature (MSByte)

//LED Junction Temperature need to be calculated according to LED On/Off and current status
If LED is on:
    LED_Junction_Temp = (Data[1]*256 + Data[0])/10 + DW*Vol*LED_I/1000
Else if LED is off: 
    LED_Junction_Temp = (Data[1]*256 + Data[0])/10

Note:
- DW is the LED thermal resister is 1.6 (°C/W)
- Vol is the LED operating voltage is about 4V
- LED_I is the LED current (mA)

// (4) Projector Image Flip Setting
Command : 0x14 // Write Projector Image Flip Setting
Data[0] = 0x00 // No Flip
Command : 0x14 // Write Projector Image Flip Setting
Data[0] = 0x02 // Horizontal Flip
Command : 0x14 // Write Projector Image Flip Setting
Data[0] = 0x04 // Vertical Flip
Command : 0x14 // Write Projector Image Flip Setting
Data[0] = 0x06 // Horiontal and Veritcal Flip


// (5) Read Light Sensor Value
Command format
    Slave device address: 0x39 [0x73 (Read)]
    1st Byte = Command
    2nd Byte = Data [0]
    3rd Byte = Data [1]

Step 1: write command
    Command: 0xAC
Step 2: read data
    Data [0] Light sensor value (LSByte)
    Data [1] Light sensor value (MSByte)
    Light_Sensor = (Data[1]*256 + Data[0])

//Note: Suggest to read the light sensor value with a fixed pattern. Light sensor value may differ with different patterns.

// (6) Projector Source and Test Patterns Selection
a. HDMI
    Command : 0x05 // Projector input source selection
    Data[0] = 0x00 // HDMI input source
b. Ramp
    Command : 0x05 // Projector input source selection
    Data[0] = 0x01 // Test Pattern
    Command : 0x0b // Pattern type selection
    Data[0] = 0x01 //Ramp
    Data[1] = 0x70
    Data[2] = 0x00
    Data[3] = 0xff
    Data[4] = 0x00
    Data[5] = 0x00
c. Checker
    Command : 0x05 // Projector input source selection
    Data[0] = 0x01 // Test Pattern
    Command : 0x0b // Pattern type selection
    Data[0] = 0x07 //Checker
    Data[1] = 0x70
    Data[2] = 0x04
    Data[3] = 0x00
    Data[4] = 0x04
    Data[5] = 0x00