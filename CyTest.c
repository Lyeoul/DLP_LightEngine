/*
 * DLP Projector Control Application
 * Control DLP projector via I2C using Cypress USB Serial Library
 *
 * Device I2C Address: 0x1B (Write: 0x36, Read: 0x37)
 * Light Sensor I2C Address: 0x39 (Read: 0x73)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "CyUSBSerial.h"

// DLP I2C addresses
#define DLP_I2C_ADDR        0x1B
#define LIGHT_SENSOR_ADDR   0x39

// DLP Commands
#define CMD_LED_CURRENT     0x54
#define CMD_LED_ENABLE      0x52
#define CMD_LED_TEMP        0xD6
#define CMD_IMAGE_FLIP      0x14
#define CMD_LIGHT_SENSOR    0xAC
#define CMD_SOURCE_SELECT   0x05
#define CMD_PATTERN_TYPE    0x0b

// LED Enable/Disable values
#define LED_ENABLE          0x02
#define LED_DISABLE         0x00

// Image flip modes
#define FLIP_NONE           0x00
#define FLIP_HORIZONTAL     0x02
#define FLIP_VERTICAL       0x04
#define FLIP_BOTH           0x06

// Source selection
#define SOURCE_HDMI         0x00
#define SOURCE_TEST_PATTERN 0x01

// Pattern types
#define PATTERN_RAMP        0x01
#define PATTERN_CHECKER     0x07

// LED thermal parameters
#define LED_THERMAL_RESIST  1.6  // °C/W
#define LED_VOLTAGE         4.0  // V
#define CURRENT_STEP        5.8593 // mA per step

// Global handle
CY_HANDLE cyHandle = NULL;

/*
 * Initialize USB Serial device and configure I2C
 */
CY_RETURN_STATUS InitializeDevice(UINT8 deviceNumber)
{
    CY_RETURN_STATUS status;
    CY_DEVICE_INFO deviceInfo;
    CY_I2C_CONFIG i2cConfig;
    UINT8 numDevices = 0;

    printf("Initializing Cypress USB Serial Library...\n");

    // Get list of devices
    status = CyGetListofDevices(&numDevices);
    if (status != CY_SUCCESS) {
        printf("Error: CyGetListofDevices failed with status %d\n", status);
        return status;
    }

    printf("Number of devices found: %d\n", numDevices);
    if (numDevices == 0) {
        printf("Error: No devices found\n");
        return CY_ERROR_DEVICE_NOT_FOUND;
    }

    // Get device info
    status = CyGetDeviceInfo(deviceNumber, &deviceInfo);
    if (status != CY_SUCCESS) {
        printf("Error: CyGetDeviceInfo failed with status %d\n", status);
        return status;
    }

    printf("Device Info:\n");
    printf("  VID: 0x%04X\n", deviceInfo.vidPid.vid);
    printf("  PID: 0x%04X\n", deviceInfo.vidPid.pid);
    printf("  Manufacturer: %s\n", deviceInfo.manufacturerName);
    printf("  Product: %s\n", deviceInfo.productName);
    printf("  Serial: %s\n", deviceInfo.serialNum);
    printf("  Number of Interfaces: %d\n", deviceInfo.numInterfaces);

    // Open device
    status = CyOpen(deviceNumber, 0, &cyHandle);
    if (status != CY_SUCCESS) {
        printf("Error: CyOpen failed with status %d\n", status);
        return status;
    }

    printf("Device opened successfully\n");

    // Configure I2C (100 kHz)
    i2cConfig.frequency = 100000; // 100 kbit/s
    i2cConfig.slaveAddress = DLP_I2C_ADDR;
    i2cConfig.isMaster = true;
    i2cConfig.isClockStretch = false;

    status = CySetI2cConfig(cyHandle, &i2cConfig);
    if (status != CY_SUCCESS) {
        printf("Error: CySetI2cConfig failed with status %d\n", status);
        CyClose(cyHandle);
        return status;
    }

    printf("I2C configured successfully (100 kHz, Master mode)\n");

    return CY_SUCCESS;
}

/*
 * Close USB Serial device
 */
void CloseDevice(void)
{
    if (cyHandle != NULL) {
        CyClose(cyHandle);
        cyHandle = NULL;
        printf("Device closed\n");
    }
}

/*
 * Write I2C command to DLP
 */
CY_RETURN_STATUS WriteI2CCommand(UINT8 command, UINT8 *data, UINT32 dataLen)
{
    CY_RETURN_STATUS status;
    CY_I2C_DATA_CONFIG i2cDataConfig;
    CY_DATA_BUFFER writeBuffer;
    UINT8 buffer[32];

    if (dataLen > 30) {
        printf("Error: Data length too large\n");
        return CY_ERROR_INVALID_PARAMETER;
    }

    // Prepare buffer: [Command][Data...]
    buffer[0] = command;
    if (data != NULL && dataLen > 0) {
        memcpy(&buffer[1], data, dataLen);
    }

    // Configure I2C transaction
    i2cDataConfig.slaveAddress = DLP_I2C_ADDR;
    i2cDataConfig.isStopBit = true;
    i2cDataConfig.isNakBit = false;

    // Prepare write buffer
    writeBuffer.buffer = buffer;
    writeBuffer.length = 1 + dataLen;
    writeBuffer.transferCount = 0;

    // Write to I2C
    status = CyI2cWrite(cyHandle, &i2cDataConfig, &writeBuffer, 5000);

    if (status != CY_SUCCESS) {
        printf("Error: I2C write failed with status %d\n", status);
        return status;
    }

    printf("I2C Write Success: Command 0x%02X, %d bytes written\n", command, writeBuffer.transferCount);
    return CY_SUCCESS;
}

/*
 * Read I2C data from DLP
 */
CY_RETURN_STATUS ReadI2CData(UINT8 command, UINT8 *data, UINT32 dataLen)
{
    CY_RETURN_STATUS status;
    CY_I2C_DATA_CONFIG i2cDataConfig;
    CY_DATA_BUFFER writeBuffer, readBuffer;
    UINT8 cmdBuffer[1];

    // First, write the command
    cmdBuffer[0] = command;
    i2cDataConfig.slaveAddress = DLP_I2C_ADDR;
    i2cDataConfig.isStopBit = false; // Don't stop, continue with read
    i2cDataConfig.isNakBit = false;

    writeBuffer.buffer = cmdBuffer;
    writeBuffer.length = 1;
    writeBuffer.transferCount = 0;

    status = CyI2cWrite(cyHandle, &i2cDataConfig, &writeBuffer, 5000);
    if (status != CY_SUCCESS) {
        printf("Error: I2C write command failed with status %d\n", status);
        return status;
    }

    // Now read the data
    i2cDataConfig.slaveAddress = DLP_I2C_ADDR;
    i2cDataConfig.isStopBit = true;
    i2cDataConfig.isNakBit = true;

    readBuffer.buffer = data;
    readBuffer.length = dataLen;
    readBuffer.transferCount = 0;

    status = CyI2cRead(cyHandle, &i2cDataConfig, &readBuffer, 5000);
    if (status != CY_SUCCESS) {
        printf("Error: I2C read failed with status %d\n", status);
        return status;
    }

    printf("I2C Read Success: %d bytes read\n", readBuffer.transferCount);
    return CY_SUCCESS;
}

/*
 * Read I2C data from Light Sensor (different address)
 */
CY_RETURN_STATUS ReadLightSensor(UINT8 command, UINT8 *data, UINT32 dataLen)
{
    CY_RETURN_STATUS status;
    CY_I2C_DATA_CONFIG i2cDataConfig;
    CY_DATA_BUFFER writeBuffer, readBuffer;
    UINT8 cmdBuffer[1];

    // Write command
    cmdBuffer[0] = command;
    i2cDataConfig.slaveAddress = LIGHT_SENSOR_ADDR;
    i2cDataConfig.isStopBit = false;
    i2cDataConfig.isNakBit = false;

    writeBuffer.buffer = cmdBuffer;
    writeBuffer.length = 1;
    writeBuffer.transferCount = 0;

    status = CyI2cWrite(cyHandle, &i2cDataConfig, &writeBuffer, 5000);
    if (status != CY_SUCCESS) {
        printf("Error: Light sensor write command failed with status %d\n", status);
        return status;
    }

    // Read data
    i2cDataConfig.slaveAddress = LIGHT_SENSOR_ADDR;
    i2cDataConfig.isStopBit = true;
    i2cDataConfig.isNakBit = true;

    readBuffer.buffer = data;
    readBuffer.length = dataLen;
    readBuffer.transferCount = 0;

    status = CyI2cRead(cyHandle, &i2cDataConfig, &readBuffer, 5000);
    if (status != CY_SUCCESS) {
        printf("Error: Light sensor read failed with status %d\n", status);
        return status;
    }

    printf("Light Sensor Read Success: %d bytes read\n", readBuffer.transferCount);
    return CY_SUCCESS;
}

/*
 * (1) Set LED Current
 * Current range: 5.8593 mA to ~6000 mA
 * Formula: Current(mA) = [(Bit value+1)/1024] x 6000
 */
CY_RETURN_STATUS SetLEDCurrent(float currentMA)
{
    UINT16 bitValue;
    UINT8 data[6];

    // Calculate bit value
    // current = [(bitValue+1)/1024] * 6000
    // bitValue = (current * 1024 / 6000) - 1
    bitValue = (UINT16)((currentMA / CURRENT_STEP) - 1);

    if (bitValue < 1 || bitValue > 1023) {
        printf("Error: Current out of range (5.86 mA to 6000 mA)\n");
        return CY_ERROR_INVALID_PARAMETER;
    }

    printf("Setting LED current to %.2f mA (bit value: 0x%04X)\n", currentMA, bitValue);

    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = bitValue & 0xFF;        // LSB
    data[3] = (bitValue >> 8) & 0xFF; // MSB
    data[4] = 0x00;
    data[5] = 0x00;

    return WriteI2CCommand(CMD_LED_CURRENT, data, 6);
}

/*
 * (2) Enable/Disable LED
 */
CY_RETURN_STATUS SetLEDEnable(BOOL enable)
{
    UINT8 data[1];

    printf("%s LED...\n", enable ? "Enabling" : "Disabling");

    data[0] = enable ? LED_ENABLE : LED_DISABLE;

    return WriteI2CCommand(CMD_LED_ENABLE, data, 1);
}

/*
 * (3) Read LED Junction Temperature
 * Returns temperature in degrees Celsius
 */
CY_RETURN_STATUS ReadLEDTemperature(float *temperature, BOOL ledOn, float currentMA)
{
    CY_RETURN_STATUS status;
    UINT8 data[2];
    UINT16 tempRaw;
    float tempBase;

    printf("Reading LED junction temperature...\n");

    status = ReadI2CData(CMD_LED_TEMP, data, 2);
    if (status != CY_SUCCESS) {
        return status;
    }

    // Calculate temperature
    tempRaw = (UINT16)(data[1] << 8) | data[0];
    tempBase = tempRaw / 10.0f;

    if (ledOn) {
        // Add thermal rise: DW * Vol * LED_I / 1000
        float thermalRise = LED_THERMAL_RESIST * LED_VOLTAGE * currentMA / 1000.0f;
        *temperature = tempBase + thermalRise;
    } else {
        *temperature = tempBase;
    }

    printf("LED Temperature: %.2f °C (Raw: %d, Base: %.2f °C)\n",
           *temperature, tempRaw, tempBase);

    return CY_SUCCESS;
}

/*
 * (4) Set Image Flip
 */
CY_RETURN_STATUS SetImageFlip(UINT8 flipMode)
{
    UINT8 data[1];
    const char *flipStr;

    switch (flipMode) {
        case FLIP_NONE:
            flipStr = "No Flip";
            break;
        case FLIP_HORIZONTAL:
            flipStr = "Horizontal Flip";
            break;
        case FLIP_VERTICAL:
            flipStr = "Vertical Flip";
            break;
        case FLIP_BOTH:
            flipStr = "Horizontal and Vertical Flip";
            break;
        default:
            printf("Error: Invalid flip mode\n");
            return CY_ERROR_INVALID_PARAMETER;
    }

    printf("Setting image flip: %s\n", flipStr);

    data[0] = flipMode;

    return WriteI2CCommand(CMD_IMAGE_FLIP, data, 1);
}

/*
 * (5) Read Light Sensor Value
 */
CY_RETURN_STATUS ReadLightSensorValue(UINT16 *value)
{
    CY_RETURN_STATUS status;
    UINT8 data[2];

    printf("Reading light sensor value...\n");

    status = ReadLightSensor(CMD_LIGHT_SENSOR, data, 2);
    if (status != CY_SUCCESS) {
        return status;
    }

    *value = (UINT16)(data[1] << 8) | data[0];

    printf("Light Sensor Value: %d (0x%04X)\n", *value, *value);

    return CY_SUCCESS;
}

/*
 * (6a) Select HDMI Input
 */
CY_RETURN_STATUS SelectHDMIInput(void)
{
    UINT8 data[1];

    printf("Selecting HDMI input source...\n");

    data[0] = SOURCE_HDMI;

    return WriteI2CCommand(CMD_SOURCE_SELECT, data, 1);
}

/*
 * (6b) Select Ramp Test Pattern
 */
CY_RETURN_STATUS SelectRampPattern(void)
{
    CY_RETURN_STATUS status;
    UINT8 data[6];

    printf("Selecting Ramp test pattern...\n");

    // First, select test pattern source
    data[0] = SOURCE_TEST_PATTERN;
    status = WriteI2CCommand(CMD_SOURCE_SELECT, data, 1);
    if (status != CY_SUCCESS) {
        return status;
    }

    // Configure ramp pattern
    data[0] = PATTERN_RAMP;
    data[1] = 0x70;
    data[2] = 0x00;
    data[3] = 0xff;
    data[4] = 0x00;
    data[5] = 0x00;

    return WriteI2CCommand(CMD_PATTERN_TYPE, data, 6);
}

/*
 * (6c) Select Checker Test Pattern
 */
CY_RETURN_STATUS SelectCheckerPattern(void)
{
    CY_RETURN_STATUS status;
    UINT8 data[6];

    printf("Selecting Checker test pattern...\n");

    // First, select test pattern source
    data[0] = SOURCE_TEST_PATTERN;
    status = WriteI2CCommand(CMD_SOURCE_SELECT, data, 1);
    if (status != CY_SUCCESS) {
        return status;
    }

    // Configure checker pattern
    data[0] = PATTERN_CHECKER;
    data[1] = 0x70;
    data[2] = 0x04;
    data[3] = 0x00;
    data[4] = 0x04;
    data[5] = 0x00;

    return WriteI2CCommand(CMD_PATTERN_TYPE, data, 6);
}

/*
 * Display menu
 */
void DisplayMenu(void)
{
    printf("\n");
    printf("========================================\n");
    printf("   DLP Projector Control Menu\n");
    printf("========================================\n");
    printf("1.  Set LED Current\n");
    printf("2.  Enable LED\n");
    printf("3.  Disable LED\n");
    printf("4.  Read LED Temperature\n");
    printf("5.  Set Image Flip (No Flip)\n");
    printf("6.  Set Image Flip (Horizontal)\n");
    printf("7.  Set Image Flip (Vertical)\n");
    printf("8.  Set Image Flip (Both)\n");
    printf("9.  Read Light Sensor Value\n");
    printf("10. Select HDMI Input\n");
    printf("11. Select Ramp Test Pattern\n");
    printf("12. Select Checker Test Pattern\n");
    printf("0.  Exit\n");
    printf("========================================\n");
    printf("Select option: ");
}

/*
 * Main function
 */
int main(int argc, char *argv[])
{
    CY_RETURN_STATUS status;
    int choice;
    float current, temperature;
    UINT16 lightValue;
    BOOL ledOn = FALSE;
    float currentMA = 0;

    printf("========================================\n");
    printf("  DLP Projector Control Application\n");
    printf("========================================\n\n");

    // Initialize device (use device 0)
    status = InitializeDevice(0);
    if (status != CY_SUCCESS) {
        printf("Failed to initialize device. Exiting...\n");
        return -1;
    }

    // Main loop
    while (1) {
        DisplayMenu();

        if (scanf("%d", &choice) != 1) {
            // Clear input buffer
            while (getchar() != '\n');
            printf("Invalid input. Please try again.\n");
            continue;
        }

        switch (choice) {
            case 1: // Set LED Current
                printf("Enter LED current in mA (5.86 to 6000): ");
                if (scanf("%f", &current) == 1) {
                    status = SetLEDCurrent(current);
                    if (status == CY_SUCCESS) {
                        currentMA = current;
                    }
                } else {
                    printf("Invalid input\n");
                    while (getchar() != '\n');
                }
                break;

            case 2: // Enable LED
                status = SetLEDEnable(TRUE);
                if (status == CY_SUCCESS) {
                    ledOn = TRUE;
                }
                break;

            case 3: // Disable LED
                status = SetLEDEnable(FALSE);
                if (status == CY_SUCCESS) {
                    ledOn = FALSE;
                }
                break;

            case 4: // Read LED Temperature
                status = ReadLEDTemperature(&temperature, ledOn, currentMA);
                break;

            case 5: // No Flip
                status = SetImageFlip(FLIP_NONE);
                break;

            case 6: // Horizontal Flip
                status = SetImageFlip(FLIP_HORIZONTAL);
                break;

            case 7: // Vertical Flip
                status = SetImageFlip(FLIP_VERTICAL);
                break;

            case 8: // Both Flip
                status = SetImageFlip(FLIP_BOTH);
                break;

            case 9: // Read Light Sensor
                status = ReadLightSensorValue(&lightValue);
                break;

            case 10: // HDMI Input
                status = SelectHDMIInput();
                break;

            case 11: // Ramp Pattern
                status = SelectRampPattern();
                break;

            case 12: // Checker Pattern
                status = SelectCheckerPattern();
                break;

            case 0: // Exit
                printf("Exiting...\n");
                CloseDevice();
                return 0;

            default:
                printf("Invalid option. Please try again.\n");
                break;
        }

        printf("\n");
    }

    CloseDevice();
    return 0;
}
