#include <Arduino.h>
#include "SensorManager.h"

// Initial simulated values
static float temperature = 28.5;
static float humidity = 62.0;
static float pressure = 1013.25;
static float gas = 125.0;

static float ax = 0.02;
static float ay = -0.01;
static float az = 9.81;

// Static packet counter for sensor manager
static uint32_t sensorPacketCounter = 0;

void SensorManager::begin()
{
    randomSeed(analogRead(0));
}

//------------------------------------------------------------
// Read Sensors
//------------------------------------------------------------
// Assembles a complete IoTPacket from simulated sensor data.
// This is the main entry point for sensor data acquisition.
//------------------------------------------------------------

IoTPacket SensorManager::readSensors()
{
    IoTPacket packet;

    // Packet metadata
    packet.packetID = ++sensorPacketCounter;
    packet.timestamp = millis();

    // Environmental sensors
    packet.temperature = simulateTemperature();
    packet.humidity = simulateHumidity();
    packet.pressure = simulatePressure();
    packet.gas = simulateGas();

    // Motion sensors
    packet.accelX = simulateAccelX();
    packet.accelY = simulateAccelY();
    packet.accelZ = simulateAccelZ();

    // System parameters
    packet.battery = random(15, 101);
    packet.cpuLoad = random(10, 96);
    packet.priority = random(1, 4);
    packet.threatLevel = random(1, 4);

    // Default AES mode (will be set by Decision Engine)
    packet.aesMode = 0;

    return packet;
}

//------------------------------------------------------------
// Validate Packet
//------------------------------------------------------------
// Checks that all sensor values are within expected ranges.
// Returns true if packet is valid, false otherwise.
//------------------------------------------------------------

bool SensorManager::validatePacket(IoTPacket &packet)
{
    // Temperature: 25.0 - 35.0 C
    if(packet.temperature < 25.0 || packet.temperature > 35.0)
        return false;

    // Humidity: 40 - 80 %
    if(packet.humidity < 40.0 || packet.humidity > 80.0)
        return false;

    // Pressure: 1008 - 1018 hPa
    if(packet.pressure < 1008.0 || packet.pressure > 1018.0)
        return false;

    // Gas: 100 - 200
    if(packet.gas < 100.0 || packet.gas > 200.0)
        return false;

    // Accelerometer X: -2 to +2 g
    if(packet.accelX < -2.0 || packet.accelX > 2.0)
        return false;

    // Accelerometer Y: -2 to +2 g
    if(packet.accelY < -2.0 || packet.accelY > 2.0)
        return false;

    // Accelerometer Z: 9 to 10 g (gravity)
    if(packet.accelZ < 9.0 || packet.accelZ > 10.0)
        return false;

    // Battery: 15 - 100 %
    if(packet.battery < 15 || packet.battery > 100)
        return false;

    // CPU Load: 10 - 95 %
    if(packet.cpuLoad < 10 || packet.cpuLoad > 95)
        return false;

    // Priority: 1 - 3
    if(packet.priority < 1 || packet.priority > 3)
        return false;

    // Threat Level: 1 - 3
    if(packet.threatLevel < 1 || packet.threatLevel > 3)
        return false;

    return true;
}

//------------------------------------------------------------

float SensorManager::simulateTemperature()
{
    temperature += random(-2,3) * 0.1;

    if(temperature < 25.0) temperature = 25.0;
    if(temperature > 35.0) temperature = 35.0;

    return temperature;
}

//------------------------------------------------------------

float SensorManager::simulateHumidity()
{
    humidity += random(-3,4) * 0.2;

    if(humidity < 40) humidity = 40;
    if(humidity > 80) humidity = 80;

    return humidity;
}

//------------------------------------------------------------

float SensorManager::simulatePressure()
{
    pressure += random(-2,3) * 0.3;

    if(pressure < 1008)
        pressure = 1008;

    if(pressure > 1018)
        pressure = 1018;

    return pressure;
}

//------------------------------------------------------------

float SensorManager::simulateGas()
{
    gas += random(-4,5);

    if(gas < 100)
        gas = 100;

    if(gas > 200)
        gas = 200;

    return gas;
}

//------------------------------------------------------------

float SensorManager::simulateAccelX()
{
    ax += random(-2,3)*0.01;

    if(ax > 2)
        ax = 2;

    if(ax < -2)
        ax = -2;

    return ax;
}

//------------------------------------------------------------

float SensorManager::simulateAccelY()
{
    ay += random(-2,3)*0.01;

    if(ay > 2)
        ay = 2;

    if(ay < -2)
        ay = -2;

    return ay;
}

//------------------------------------------------------------

float SensorManager::simulateAccelZ()
{
    az += random(-1,2)*0.02;

    if(az > 10)
        az = 10;

    if(az < 9)
        az = 9;

    return az;
}