#ifndef CONFIG_H
#define CONFIG_H


#define BATTERY_LOW        30
#define BATTERY_MEDIUM     75
#define BATTERY_HIGH       100

//===========================
// CPU Thresholds
//===========================

#define CPU_LOW            40
#define CPU_MEDIUM         80

//===========================
// Memory Threshold
//===========================

#define MEMORY_LOW         30

//===========================
// AES Modes
//===========================

#define AES128_MODE        128
#define AES192_MODE        192
#define AES256_MODE        256

//===========================
// Packet Priority
//===========================

#define LOW_PRIORITY       1
#define MEDIUM_PRIORITY    2
#define HIGH_PRIORITY      3
#define CRITICAL_PRIORITY  4

//===========================
// Timing
//===========================

#define SENSOR_INTERVAL    1000
#define LOG_INTERVAL       2000

//===========================
// Simulation
//===========================

#define SIMULATION_MODE    true

#endif