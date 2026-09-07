#!/usr/bin/env python3
"""
Adaptive AES Data Collector
Reads Serial output from ESP32 and saves to CSV.

Usage:
    python data_collector.py

Requirements:
    pip install pyserial
"""

import serial as pyserial  # Fixed: alias the import
import csv
import time
import re
import sys
from datetime import datetime

# ============================================
# CONFIGURATION — CHANGE THESE VALUES
# ============================================

SERIAL_PORT = "COM15"
BAUD_RATE = 115200
OUTPUT_FILE = "adaptive_aes_results.csv"
NUM_PACKETS = 500
TIMEOUT = 5

# ============================================
# CSV COLUMN HEADERS
# ============================================

CSV_HEADERS = [
    "PacketID",
    "Timestamp",
    "Temperature",
    "Humidity",
    "Pressure",
    "Gas",
    "AccelX",
    "AccelY",
    "AccelZ",
    "Battery",
    "CPU_Load",
    "Priority",
    "Threat_Level",
    "Decision_Score",
    "Selected_AES_Mode",
    "Encryption_Time_us",
    "Energy_mJ",
    "AES128_Time_us",
    "AES192_Time_us",
    "AES256_Time_us",
    "Adaptive_Time_us",
    "AES128_Energy_mJ",
    "AES192_Energy_mJ",
    "AES256_Energy_mJ",
    "Adaptive_Energy_mJ",
    "Avalanche_Effect",
    "Entropy",
    "Security_Score",
    "Decryption_Success"
]

# ============================================
# FIELD PARSING MAP
# ============================================

FIELD_MAP = {
    "Packet ID": "PacketID",
    "Timestamp": "Timestamp",
    "Temperature (C)": "Temperature",
    "Humidity (%)": "Humidity",
    "Pressure (hPa)": "Pressure",
    "Gas": "Gas",
    "Acceleration X": "AccelX",
    "Acceleration Y": "AccelY",
    "Acceleration Z": "AccelZ",
    "Battery (%)": "Battery",
    "CPU Load (%)": "CPU_Load",
    "Priority Level": "Priority",
    "Threat Level": "Threat_Level",
    "Decision Score": "Decision_Score",
    "Selected AES Mode": "Selected_AES_Mode",
    "Encryption Time (us)": "Encryption_Time_us",
    "Energy (mJ)": "Energy_mJ",
    "AES-128 Time (us)": "AES128_Time_us",
    "AES-192 Time (us)": "AES192_Time_us",
    "AES-256 Time (us)": "AES256_Time_us",
    "Adaptive Time (us)": "Adaptive_Time_us",
    "AES-128 Energy (mJ)": "AES128_Energy_mJ",
    "AES-192 Energy (mJ)": "AES192_Energy_mJ",
    "AES-256 Energy (mJ)": "AES256_Energy_mJ",
    "Adaptive Energy (mJ)": "Adaptive_Energy_mJ",
    "Avalanche Effect (%)": "Avalanche_Effect",
    "Entropy (bits/byte)": "Entropy",
    "Security Score": "Security_Score",
    "Decryption Status": "Decryption_Success"
}

# ============================================
# MAIN FUNCTION
# ============================================

def connect_serial(port, baud_rate, timeout):
    """Connect to ESP32 Serial port."""
    try:
        ser = pyserial.Serial(port, baud_rate, timeout=timeout)
        time.sleep(2)
        print(f"[OK] Connected to {port} at {baud_rate} baud")
        return ser
    except pyserial.SerialException as e:
        print(f"[ERROR] Cannot open {port}: {e}")
        print("\nAvailable ports:")
        import serial.tools.list_ports
        for p in serial.tools.list_ports.comports():
            print(f"  - {p.device} ({p.description})")
        sys.exit(1)


def parse_line(line):
    """Parse a single line from Serial output."""
    line = line.strip()
    if not line:
        return None, None
    
    match = re.match(r'^(.+?)\s*:\s*(.+)$', line)
    if match:
        label = match.group(1).strip()
        value = match.group(2).strip()
        return label, value
    
    return None, None


def clean_value(value):
    """Clean and convert value for CSV."""
    value = value.strip()
    
    if value.startswith("AES-"):
        value = value.replace("AES-", "")
    
    if "SUCCESS" in value:
        return "1"
    if "FAIL" in value:
        return "0"
    
    return value


def main():
    print("=" * 60)
    print("Adaptive AES Data Collector")
    print("=" * 60)
    print(f"Port: {SERIAL_PORT}")
    print(f"Baud: {BAUD_RATE}")
    print(f"Output: {OUTPUT_FILE}")
    print(f"Target: {NUM_PACKETS} packets")
    print("=" * 60)
    print()
    
    ser = connect_serial(SERIAL_PORT, BAUD_RATE, TIMEOUT)
    
    csv_file = open(OUTPUT_FILE, "w", newline="", encoding="utf-8")
    csv_writer = csv.writer(csv_file)
    csv_writer.writerow(CSV_HEADERS)
    
    packet_count = 0
    current_packet = {}
    packet_complete = False
    
    print(f"Collecting {NUM_PACKETS} packets...")
    print("Press Ctrl+C to stop early\n")
    
    try:
        while packet_count < NUM_PACKETS:
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            
            if not line:
                continue
            
            label, value = parse_line(line)
            
            if label and value:
                if label in FIELD_MAP:
                    csv_column = FIELD_MAP[label]
                    clean_val = clean_value(value)
                    current_packet[csv_column] = clean_val
                    
                    if csv_column == "Decryption_Success":
                        packet_complete = True
            
            if packet_complete and "Decryption_Success" in current_packet:
                row = [current_packet.get(col, "") for col in CSV_HEADERS]
                csv_writer.writerow(row)
                csv_file.flush()
                
                packet_count += 1
                
                if packet_count % 25 == 0:
                    print(f"  Collected {packet_count}/{NUM_PACKETS} packets")
                
                current_packet = {}
                packet_complete = False
    
    except KeyboardInterrupt:
        print(f"\n[STOPPED] Collected {packet_count} packets (user interrupted)")
    
    except Exception as e:
        print(f"\n[ERROR] {e}")
    
    finally:
        csv_file.close()
        ser.close()
        print(f"\n{'=' * 60}")
        print(f"Data saved to: {OUTPUT_FILE}")
        print(f"Total packets: {packet_count}")
        print(f"{'=' * 60}")


if __name__ == "__main__":
    main()