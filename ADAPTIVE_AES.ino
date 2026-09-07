#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include <Adafruit_BME680.h>
#include <Adafruit_INA219.h>

#include "Config.h"
#include "Packet.h"
#include "DecisionEngine.h"
#include "AESManager.h"
#include "DecisionReport.h"
#include "PerformanceLogger.h"
#include "SecurityAnalyzer.h"
#include "CSVLogger.h"
#include "ResultAnalyzer.h"
#include "Comparator.h"

//------------------------------------------------------
// CPU Load Measurement Variables
//------------------------------------------------------
volatile unsigned long idleTaskCount = 0;
unsigned long lastIdleCount = 0;
unsigned long lastCPUTime = 0;
int realCPULoad = 0;

//------------------------------------------------------
// FreeRTOS Idle Hook
//------------------------------------------------------
void vApplicationIdleHook(void)
{
    idleTaskCount++;
}

//------------------------------------------------------
// Measure Real CPU Load
//------------------------------------------------------
int measureCPULoad()
{
    unsigned long currentTime = millis();
    unsigned long currentIdleCount = idleTaskCount;
    unsigned long timeElapsed = currentTime - lastCPUTime;
    
    if(timeElapsed >= 1000)
    {
        unsigned long idleIncrement = currentIdleCount - lastIdleCount;
        unsigned long maxIdlePerSec = 250000;
        
        int idlePercentage = (idleIncrement * 100) / maxIdlePerSec;
        
        if(idlePercentage > 100) idlePercentage = 100;
        if(idlePercentage < 0) idlePercentage = 0;
        
        realCPULoad = 100 - idlePercentage;
        
        if(realCPULoad < 0) realCPULoad = 0;
        if(realCPULoad > 100) realCPULoad = 100;
        
        lastIdleCount = currentIdleCount;
        lastCPUTime = currentTime;
    }
    
    return realCPULoad;
}

//------------------------------------------------------
// Global Objects
//------------------------------------------------------
MPU6050 mpu(Wire);
Adafruit_BME680 bme;
Adafruit_INA219 ina219(0x40);

DecisionEngine decisionEngine;
AESManager aesManager;
Comparator comparator;

ComparisonReport comparison;
PerformanceLogger performanceLogger;
SecurityAnalyzer securityAnalyzer;
IoTPacket packet;
DecisionReport report;
CSVLogger csvLogger;
ResultAnalyzer resultAnalyzer;

uint8_t plaintext[PACKET_BUFFER_SIZE];

//------------------------------------------------------
// Setup
//------------------------------------------------------

void setup()
{
    setCpuFrequencyMhz(80);
    Serial.begin(115200);
    delay(1000);

    Wire.begin(21, 22);

    //--------------------------------------------------
    // MPU6050 Initialization
    //--------------------------------------------------
    byte status = mpu.begin();
    Serial.print("MPU6050 status: ");
    Serial.println(status);
    
    if(status == 0)
    {
        Serial.println("MPU6050 connected!");
        mpu.calcOffsets(true, true);
    }
    else
    {
        Serial.println("MPU6050 FAILED!");
    }

    //--------------------------------------------------
    // BME680 Initialization
    //--------------------------------------------------
    if(bme.begin(0x77))
    {
        Serial.println("BME680 connected!");
        bme.setTemperatureOversampling(BME680_OS_8X);
        bme.setHumidityOversampling(BME680_OS_2X);
        bme.setPressureOversampling(BME680_OS_4X);
        bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
        bme.setGasHeater(320, 150);
    }
    else
    {
        Serial.println("BME680 FAILED!");
    }

    //--------------------------------------------------
    // INA219 Initialization
    //--------------------------------------------------
    if(ina219.begin())
    {
        Serial.println("INA219 connected!");
    }
    else
    {
        Serial.println("INA219 FAILED!");
    }

    randomSeed(analogRead(0));
    
    lastIdleCount = idleTaskCount;
    lastCPUTime = millis();
    
    decisionEngine.begin();
    aesManager.begin();
    comparator.begin();
    performanceLogger.begin();
    securityAnalyzer.begin();
    csvLogger.begin();
    resultAnalyzer.begin();

    Serial.println();
    Serial.println("========================================================");
    Serial.println(" Adaptive AES for Energy-Efficient IoT & Embedded System");
    Serial.println(" ESP32 + MPU6050 + BME680 + INA219");
    Serial.println("========================================================");
}

//------------------------------------------------------
// Main Loop
//------------------------------------------------------

void loop()
{
    packet.packetID++;
    packet.timestamp = millis();

    //--------------------------------------------------
    // MPU6050
    //--------------------------------------------------
    mpu.update();
    packet.accelX = mpu.getAccX();
    packet.accelY = mpu.getAccY();
    packet.accelZ = mpu.getAccZ();

    //--------------------------------------------------
    // BME680
    //--------------------------------------------------
    if(bme.performReading())
    {
        packet.temperature = bme.temperature;
        packet.humidity = bme.humidity;
        packet.pressure = bme.pressure / 100.0;
        packet.gas = bme.gas_resistance / 1000.0;
    }
    else
    {
        packet.temperature = 25.0;
        packet.humidity = 50.0;
        packet.pressure = 1013.0;
        packet.gas = 100.0;
    }

    //--------------------------------------------------
    // CPU Load
    //--------------------------------------------------
    packet.cpuLoad = measureCPULoad();

    //--------------------------------------------------
    // Battery from INA219 Voltage
    //--------------------------------------------------
    float busVoltage = ina219.getBusVoltage_V();
    if(busVoltage > 2.0)
    {
        packet.battery = (int)((busVoltage - 3.0) / (4.2 - 3.0) * 100);
        if(packet.battery > 100) packet.battery = 100;
        if(packet.battery < 0) packet.battery = 0;
    }
    else
    {
        packet.battery = 100;  // Fallback if USB powered
    }

    packet.priority = random(1, 4);
    packet.threatLevel = random(1, 4);

    //--------------------------------------------------
    // Comparator
    //--------------------------------------------------
    comparison = comparator.compare(packet);
    report = comparison.adaptive;
    float score = report.decisionScore;

    memcpy(plaintext, report.plaintext, PACKET_BUFFER_SIZE);
    securityAnalyzer.analyze(plaintext, report);
    aesManager.decryptPacket(report);

    performanceLogger.log(report);
    csvLogger.log(packet, report, score);
    resultAnalyzer.update(report, score);

    //--------------------------------------------------
    // DISPLAY
    //--------------------------------------------------
    Serial.println();
    Serial.println("==================================================");
    Serial.print("Packet: ");
    Serial.print(packet.packetID);
    Serial.print(" | CPU: ");
    Serial.print(packet.cpuLoad);
    Serial.print("% | Battery: ");
    Serial.print(packet.battery);
    Serial.print("% | Mode: AES-");
    Serial.println(report.selectedAES);
    Serial.println("--------------------------------------------------");

    Serial.print("MPU6050: X=");
    Serial.print(packet.accelX, 2);
    Serial.print(" Y=");
    Serial.print(packet.accelY, 2);
    Serial.print(" Z=");
    Serial.println(packet.accelZ, 2);

    Serial.print("BME680: T=");
    Serial.print(packet.temperature, 2);
    Serial.print("C H=");
    Serial.print(packet.humidity, 2);
    Serial.print("% P=");
    Serial.print(packet.pressure, 2);
    Serial.print("hPa G=");
    Serial.print(packet.gas, 2);
    Serial.println("kOhms");

    Serial.print("INA219: ");
    Serial.print(busVoltage, 3);
    Serial.print("V | ");
    Serial.print(ina219.getCurrent_mA(), 1);
    Serial.print("mA | ");
    Serial.print(ina219.getPower_mW(), 1);
    Serial.println("mW");

    Serial.println("--------------------------------------------------");
    Serial.print("Decision Score: ");
    Serial.println(score, 2);
    Serial.print("AES-128: ");
    Serial.print(comparison.fixed128.encryptionTime_us, 0);
    Serial.print("us | AES-192: ");
    Serial.print(comparison.fixed192.encryptionTime_us, 0);
    Serial.print("us | AES-256: ");
    Serial.print(comparison.fixed256.encryptionTime_us, 0);
    Serial.println("us");
    Serial.print("Adaptive: ");
    Serial.print(comparison.adaptive.encryptionTime_us, 0);
    Serial.print("us | Energy: ");
    Serial.print(report.estimatedEnergy_mJ, 6);
    Serial.println("mJ");

    Serial.println("--------------------------------------------------");
    Serial.print("Avalanche: ");
    Serial.print(report.avalancheEffect, 2);
    Serial.print("% | Entropy: ");
    Serial.print(report.entropy, 4);
    Serial.print(" | Security: ");
    Serial.println(report.securityScore, 2);

    Serial.print("Decryption: ");
    if (report.decryptionSuccess)
        Serial.println("SUCCESS - DATA MATCH");
    else
        Serial.println("FAILED");

    Serial.println("==================================================");
    Serial.println();

    if (packet.packetID % 100 == 0)
        resultAnalyzer.printSummary();

    delay(2000);
}