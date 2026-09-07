# Adaptive AES for Energy-Efficient IoT and Embedded Systems

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Status: Software Simulation](https://img.shields.io/badge/Status-Software%20Simulation-green.svg)]()
[![Language: C/C++](https://img.shields.io/badge/Language-C%2FC%2B%2B-orange.svg)]()

## Overview

**Adaptive AES for Energy-Efficient IoT and Embedded Systems** is an embedded security project that investigates the use of **adaptive AES encryption** for resource-constrained IoT devices.

Conventional IoT implementations may use a fixed AES key size regardless of the current operating condition of the device. This project explores a different approach: dynamically selecting **AES-128, AES-192, or AES-256** according to the device's operating conditions.

The current implementation is developed as a **software simulation**, with the ESP32 and sensor hardware planned for the subsequent hardware-integration phase.

### Project Objective

The main objective is to investigate whether adaptive selection of AES variants can provide a practical balance between:

* Security strength
* Encryption latency
* Computational overhead
* Energy efficiency
* IoT device operating conditions

AES encryption is applied to the **complete IoT packet**, rather than encrypting individual sensor values separately.

---

## System Concept

The system follows the following processing flow:

```text
             IoT Data
                |
                v
        Packet Formation
                |
                v
       System Telemetry
       -----------------
       Battery Level
       CPU Load
                |
                v
       Adaptive Decision
             Engine
                |
        +-------+-------+
        |       |       |
        v       v       v
     AES-128 AES-192 AES-256
        |       |       |
        +-------+-------+
                |
                v
        Packet Encryption
                |
                v
       Performance Logging
                |
                v
       Result Analysis
```

---

## Adaptive AES Decision Logic

The current adaptive mechanism evaluates the operating condition of the simulated IoT device and selects an AES variant.

### Current selection policy

| Condition                        | Selected AES |
| -------------------------------- | ------------ |
| Battery ≤ 30% OR CPU Load > 80%  | AES-128      |
| 30% < Battery ≤ 75%              | AES-192      |
| Battery > 75% AND CPU Load < 40% | AES-256      |

The objective is to use a lower computational-security configuration when device resources are constrained and a stronger configuration when sufficient resources are available.

> **Note:** These thresholds are project-defined decision parameters and are evaluated as part of the experimental methodology. They are not presented as universal security recommendations.

---

## Key Features

| Feature                     | Description                                                               |
| --------------------------- | ------------------------------------------------------------------------- |
| **Adaptive AES Selection**  | Dynamically selects AES-128, AES-192, or AES-256                          |
| **IoT Packet Encryption**   | Encrypts complete simulated IoT packets                                   |
| **Decision Engine**         | Evaluates device operating conditions                                     |
| **Performance Measurement** | Records encryption and decryption timing                                  |
| **Security Analysis**       | Evaluates avalanche effect and ciphertext entropy                         |
| **Energy Analysis**         | Estimates comparative energy consumption during simulation                |
| **CSV Logging**             | Stores experimental results for further analysis                          |
| **Python Analysis**         | Generates statistics and visualization from collected data                |
| **Modular Architecture**    | Separates decision, encryption, logging, comparison, and analysis modules |

---

# Current Development Phase

The project is currently being developed in two major stages.

### Phase 1: Software Simulation

**Current phase**

The software implementation focuses on:

* AES-128
* AES-192
* AES-256
* Adaptive decision logic
* IoT packet generation
* Encryption/decryption validation
* Performance logging
* Security analysis
* Comparative analysis
* CSV data collection
* Python-based result analysis

### Phase 2: Hardware Implementation

**Planned**

The software implementation will later be transferred to an ESP32-based hardware platform.

Planned hardware includes:

* ESP32 DevKit V1
* MPU6050
* BME680
* INA219

Hardware measurements will be used to validate the simulation results under real embedded operating conditions.

---

# Hardware Platform

| Component           | Purpose                        | Current Status               |
| ------------------- | ------------------------------ | ---------------------------- |
| **ESP32 DevKit V1** | Target embedded controller     | Planned hardware integration |
| **MPU6050**         | Motion data acquisition        | Planned                      |
| **BME680**          | Environmental data acquisition | Planned                      |
| **INA219**          | Voltage/current monitoring     | Planned                      |

The hardware phase is intentionally separated from the current software simulation so that experimental results are not presented as physical measurements before the hardware implementation is completed.

---

# IoT Packet Structure

The encryption module operates on the **complete IoT packet**.

A simplified packet contains information such as:

```text
+--------------------------------------+
| Device Information                   |
+--------------------------------------+
| Timestamp                            |
+--------------------------------------+
| Sensor / Payload Data                |
+--------------------------------------+
| System Status                        |
+--------------------------------------+
| Additional Packet Fields             |
+--------------------------------------+
```

The packet is serialized before encryption.

The current implementation uses a fixed packet structure suitable for controlled experimentation.

---

# AES Encryption

The project evaluates three AES configurations:

### AES-128

* 128-bit key
* 10 encryption rounds
* Lower computational requirement among the three configurations

### AES-192

* 192-bit key
* 12 encryption rounds
* Intermediate configuration

### AES-256

* 256-bit key
* 14 encryption rounds
* Highest key strength among the three configurations

The project compares these variants based on their measured performance within the simulation environment.

---

# Experimental Metrics

The implementation records several metrics to evaluate the adaptive approach.

### Performance

* Encryption latency
* Decryption latency
* Processing overhead
* Throughput

### Energy

* Estimated energy consumption
* Relative energy cost between AES variants
* Adaptive-mode energy comparison

### Security

* Avalanche effect
* Ciphertext entropy
* Successful decryption rate

### Adaptive Behaviour

* AES-128 selection frequency
* AES-192 selection frequency
* AES-256 selection frequency

---

# Example Software Results

The current software experiment was evaluated using a set of simulated IoT packets.

### Encryption Performance

| AES Variant | Encryption Time |
| ----------- | --------------: |
| AES-128     |      ~701.35 µs |
| AES-192     |      ~701.28 µs |
| AES-256     |      ~701.36 µs |

These values represent the current software experiment and should be interpreted within the specific implementation and execution environment.

Further hardware-based benchmarking is planned using the ESP32 platform.

### Security Validation

| Metric                  |  Current Result |
| ----------------------- | --------------: |
| Decryption Success Rate |            100% |
| Avalanche Effect        |          50.12% |
| Ciphertext Entropy      | 7.218 bits/byte |

The avalanche result is close to the commonly expected 50% behaviour for a well-performing block cipher experiment.

---

# Adaptive Mode Distribution

Example distribution from the current experiment:

| AES Mode  | Packets | Percentage |
| --------- | ------: | ---------: |
| AES-128   |      30 |       6.0% |
| AES-192   |     321 |      64.2% |
| AES-256   |     149 |      29.8% |
| **Total** | **500** |   **100%** |

The distribution reflects the simulated battery and CPU conditions supplied to the adaptive decision engine.

---

# Project Architecture

The implementation follows a modular architecture.

```text
ADAPTIVE_AES/
│
├── ADAPTIVE_AES.ino
│
├── Config.h
│
├── Packet.h
│
├── DecisionEngine/
│   └── Adaptive AES selection logic
│
├── AESManager/
│   └── Packet serialization
│   └── Encryption
│   └── Decryption
│
├── Comparator/
│   └── Fixed AES vs Adaptive AES comparison
│
├── PerformanceLogger/
│   └── Timing
│   └── Performance metrics
│
├── SecurityAnalyzer/
│   └── Avalanche analysis
│   └── Entropy analysis
│   └── Security metrics
│
├── CSVLogger/
│   └── Experimental data logging
│
├── ResultAnalyzer/
│   └── Statistical processing
│
├── SensorManager/
│   └── Simulated sensor data generation
│
├── AES_Wrappers/
│   └── AES-128
│   └── AES-192
│   └── AES-256
│
└── python/
    ├── data_collector.py
    └── analyze_results.py
```

---

# Software Requirements

### Required

* C/C++
* Arduino IDE or PlatformIO
* Python 3.x
* Git

### Python Libraries

Install the required Python packages using:

```bash
pip install -r requirements.txt
```

Typical analysis dependencies include:

```text
numpy
pandas
matplotlib
```

---

# Quick Start

## 1. Clone the Repository

```bash
git clone https://github.com/Theorder-1/adaptive-aes-iot.git
cd adaptive-aes-iot
```

## 2. Software Simulation

Run the software implementation according to the project structure.

The simulation generates IoT packets and evaluates the adaptive AES selection mechanism.

## 3. Collect Results

If using the serial-data collection workflow:

```bash
python python/data_collector.py
```

## 4. Analyze Results

```bash
python python/analyze_results.py
```

The analysis scripts process the collected CSV data and generate statistical results and graphs.

---

# Generated Analysis Graphs

The analysis module produces visualizations such as:

| File                         | Description                    |
| ---------------------------- | ------------------------------ |
| `fig1_encryption_time.png`   | AES encryption-time comparison |
| `fig2_energy_comparison.png` | Comparative energy analysis    |
| `fig3_mode_distribution.png` | Adaptive AES mode distribution |
| `fig4_security_metrics.png`  | Security metric comparison     |

---

# Comparison Methodology

The project compares two approaches:

### Fixed AES

A single AES variant is used for all IoT packets.

```text
IoT Packet
    |
    v
Fixed AES
    |
    v
Encrypted Packet
```

### Adaptive AES

The AES variant is selected according to the current operating condition.

```text
IoT Packet
    |
    v
System Conditions
    |
    v
Decision Engine
    |
    +----> AES-128
    |
    +----> AES-192
    |
    +----> AES-256
    |
    v
Encrypted Packet
```

The comparison is used to investigate whether adaptive selection can reduce computational and energy requirements while maintaining an appropriate level of encryption strength.

---

# Research Objective

The central research question is:

> **Can adaptive selection between AES-128, AES-192, and AES-256 improve the energy efficiency of resource-constrained IoT systems while maintaining reliable data protection?**

The project evaluates this question through controlled software experiments followed by planned hardware validation.

---

# Future Hardware Implementation

The next development stage will integrate the software architecture with the ESP32 platform.

Planned system:

```text
MPU6050 ─┐
         │
BME680 ──┼──> ESP32
         │      |
         │      v
         │  IoT Packet
         │      |
         │      v
         │ Adaptive AES
         │      |
         │      v
         └─> Encrypted Data
                |
                v
             Logging

INA219
   |
   v
Power Measurement
   |
   v
ESP32 Energy Analysis
```

The hardware implementation will provide physical measurements of system performance and power consumption.

---

# Repository Status

| Component                  | Status   |
| -------------------------- | -------- |
| AES-128                    | Complete |
| AES-192                    | Complete |
| AES-256                    | Complete |
| Adaptive Decision Engine   | Complete |
| IoT Packet Simulation      | Complete |
| Encryption/Decryption      | Complete |
| Security Analysis          | Complete |
| CSV Logging                | Complete |
| Python Analysis            | Complete |
| ESP32 Hardware Integration | Planned  |
| MPU6050 Integration        | Planned  |
| BME680 Integration         | Planned  |
| INA219 Power Measurement   | Planned  |

---

# Project Goals

The project aims to demonstrate:

1. Adaptive cryptographic selection for IoT systems.
2. Performance differences between AES key sizes.
3. The relationship between device conditions and encryption configuration.
4. Comparative energy behaviour of different AES variants.
5. A modular architecture suitable for embedded implementation.
6. A transition path from software simulation to physical ESP32 validation.

---

# Citation

If you reference this project in academic or technical work:

```bibtex
@techreport{adaptive-aes-2026,
    title={Adaptive AES for Energy-Efficient IoT and Embedded Systems},
    author={Theorder-1},
    year={2026},
    howpublished={GitHub},
    url={https://github.com/Theorder-1/adaptive-aes-iot}
}
```

---

# License

This project is licensed under the MIT License.

See the `LICENSE` file for details.

---

# Acknowledgments

This project uses or is inspired by the following open-source resources:

* [tiny-AES-c](https://github.com/kokke/tiny-AES-c) for lightweight AES implementation.
* [MPU6050_light](https://github.com/rfetick/MPU6050_light) for planned MPU6050 integration.
* [Adafruit INA219](https://github.com/adafruit/Adafruit_INA219) for planned power-monitoring integration.

---

# Author

**Theorder-1**

GitHub Repository:

https://github.com/Theorder-1/adaptive-aes-iot
