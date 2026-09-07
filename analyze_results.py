#!/usr/bin/env python3
"""
Adaptive AES Results Analyzer
Reads CSV and generates publication-quality graphs.
"""

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np

# Professional style
plt.style.use('seaborn-v0_8-whitegrid')
mpl.rcParams['figure.figsize'] = (10, 6)
mpl.rcParams['figure.dpi'] = 300
mpl.rcParams['font.size'] = 11
mpl.rcParams['axes.labelsize'] = 12
mpl.rcParams['axes.titlesize'] = 14
mpl.rcParams['xtick.labelsize'] = 10
mpl.rcParams['ytick.labelsize'] = 10

# Load data
df = pd.read_csv("adaptive_aes_results_80MHz.csv", sep=None, engine='python')
# Clean numeric columns
numeric_cols = [
    'AES128_Time_us', 'AES192_Time_us', 'AES256_Time_us', 'Adaptive_Time_us',
    'AES128_Energy_mJ', 'AES192_Energy_mJ', 'AES256_Energy_mJ', 'Adaptive_Energy_mJ',
    'Avalanche_Effect', 'Entropy', 'Security_Score'
]
for col in numeric_cols:
    df[col] = pd.to_numeric(df[col], errors='coerce')

df['Selected_AES_Mode'] = df['Selected_AES_Mode'].astype(str)

# ============================================
# SUMMARY STATISTICS
# ============================================

print("\n" + "="*60)
print("ADAPTIVE AES EXPERIMENT — SUMMARY STATISTICS")
print("="*60)

print(f"\nTotal Packets: {len(df)}")
print(f"Decryption Success Rate: {df['Decryption_Success'].mean()*100:.1f}%")

print("\n--- Encryption Time (µs) ---")
print(f"AES-128:  Mean={df['AES128_Time_us'].mean():.2f}, Std={df['AES128_Time_us'].std():.2f}")
print(f"AES-192:  Mean={df['AES192_Time_us'].mean():.2f}, Std={df['AES192_Time_us'].std():.2f}")
print(f"AES-256:  Mean={df['AES256_Time_us'].mean():.2f}, Std={df['AES256_Time_us'].std():.2f}")
print(f"Adaptive: Mean={df['Adaptive_Time_us'].mean():.2f}, Std={df['Adaptive_Time_us'].std():.2f}")

print("\n--- Energy (mJ) ---")
print(f"AES-128:  Mean={df['AES128_Energy_mJ'].mean():.6f}")
print(f"AES-192:  Mean={df['AES192_Energy_mJ'].mean():.6f}")
print(f"AES-256:  Mean={df['AES256_Energy_mJ'].mean():.6f}")
print(f"Adaptive: Mean={df['Adaptive_Energy_mJ'].mean():.6f}")

print("\n--- Mode Selection Frequency ---")
mode_counts = df['Selected_AES_Mode'].value_counts().sort_index()
for mode, count in mode_counts.items():
    pct = (count / len(df)) * 100
    print(f"AES-{mode}: {count} packets ({pct:.1f}%)")

print("\n--- Security Metrics ---")
print(f"Avalanche Effect: Mean={df['Avalanche_Effect'].mean():.2f}%")
print(f"Entropy: Mean={df['Entropy'].mean():.4f} bits/byte")
print(f"Security Score: Mean={df['Security_Score'].mean():.2f}")

# Energy savings
avg_256 = df['AES256_Energy_mJ'].mean()
avg_adaptive = df['Adaptive_Energy_mJ'].mean()
savings = ((avg_256 - avg_adaptive) / avg_256) * 100
print(f"\n--- Energy Savings ---")
print(f"Always AES-256: {avg_256:.6f} mJ/packet")
print(f"Adaptive AES:   {avg_adaptive:.6f} mJ/packet")
print(f"Savings:        {savings:.2f}%")

# ============================================
# GRAPH 1: Encryption Time Comparison
# ============================================

fig, ax = plt.subplots(figsize=(8, 5))

modes = ['AES-128', 'AES-192', 'AES-256', 'Adaptive']
times = [
    df['AES128_Time_us'].mean(),
    df['AES192_Time_us'].mean(),
    df['AES256_Time_us'].mean(),
    df['Adaptive_Time_us'].mean()
]
errors = [
    df['AES128_Time_us'].std(),
    df['AES192_Time_us'].std(),
    df['AES256_Time_us'].std(),
    df['Adaptive_Time_us'].std()
]

bars = ax.bar(modes, times, yerr=errors, capsize=5,
              color=['#2E86AB', '#A23B72', '#F18F01', '#6A994E'],
              edgecolor='black', linewidth=0.5)

ax.set_ylabel('Encryption Time (µs)', fontsize=12)
ax.set_title('AES Encryption Time Comparison\n(48-byte IoT Packet, ESP32 @ 240 MHz)', fontsize=14)
ax.set_xlabel('AES Mode', fontsize=12)

for bar, val in zip(bars, times):
    ax.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 2,
            f'{val:.1f}', ha='center', va='bottom', fontsize=10)

plt.tight_layout()
plt.savefig('fig1_encryption_time.png', bbox_inches='tight')
plt.show()
print("\n✅ fig1_encryption_time.png saved")

# ============================================
# GRAPH 2: Energy Comparison
# ============================================

fig, ax = plt.subplots(figsize=(8, 5))

energies = [
    df['AES128_Energy_mJ'].mean(),
    df['AES192_Energy_mJ'].mean(),
    df['AES256_Energy_mJ'].mean(),
    df['Adaptive_Energy_mJ'].mean()
]

bars = ax.bar(modes, energies, color=['#2E86AB', '#A23B72', '#F18F01', '#6A994E'],
              edgecolor='black', linewidth=0.5)

ax.set_ylabel('Energy (mJ)', fontsize=12)
ax.set_title('AES Energy Consumption Comparison\n(48-byte IoT Packet)', fontsize=14)
ax.set_xlabel('AES Mode', fontsize=12)

for bar, val in zip(bars, energies):
    ax.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.0005,
            f'{val:.6f}', ha='center', va='bottom', fontsize=9)

plt.tight_layout()
plt.savefig('fig2_energy_comparison.png', bbox_inches='tight')
plt.show()
print("✅ fig2_energy_comparison.png saved")

# ============================================
# GRAPH 3: Mode Distribution Pie Chart
# ============================================

fig, ax = plt.subplots(figsize=(7, 7))

mode_labels = [f'AES-{mode}' for mode in mode_counts.index]
mode_values = mode_counts.values
colors = ['#2E86AB', '#A23B72', '#F18F01']

wedges, texts, autotexts = ax.pie(mode_values, labels=mode_labels,
                                   autopct='%1.1f%%', colors=colors,
                                   startangle=90, explode=[0.02]*len(mode_values))

for text in texts:
    text.set_fontsize(12)
for autotext in autotexts:
    autotext.set_fontsize(11)

ax.set_title('Adaptive AES Mode Selection Distribution\n(500 Packets)', fontsize=14)

plt.tight_layout()
plt.savefig('fig3_mode_distribution.png', bbox_inches='tight')
plt.show()
print("✅ fig3_mode_distribution.png saved")

# ============================================
# GRAPH 4: Security Metrics
# ============================================

fig, axes = plt.subplots(1, 2, figsize=(12, 5))

# Avalanche
ax1 = axes[0]
avalanche_vals = [
    df[df['Selected_AES_Mode'] == '128']['Avalanche_Effect'].mean(),
    df[df['Selected_AES_Mode'] == '192']['Avalanche_Effect'].mean(),
    df[df['Selected_AES_Mode'] == '256']['Avalanche_Effect'].mean(),
    df['Avalanche_Effect'].mean()
]

bars1 = ax1.bar(modes, avalanche_vals, color=['#2E86AB', '#A23B72', '#F18F01', '#6A994E'],
                edgecolor='black', linewidth=0.5)
ax1.set_ylabel('Avalanche Effect (%)', fontsize=12)
ax1.set_title('Avalanche Effect by AES Mode', fontsize=14)
ax1.set_ylim(0, 100)
ax1.axhline(y=50, color='red', linestyle='--', label='Ideal (50%)')
ax1.legend()

for bar, val in zip(bars1, avalanche_vals):
    ax1.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 2,
            f'{val:.1f}', ha='center', va='bottom', fontsize=9)

# Entropy
ax2 = axes[1]
entropy_vals = [
    df[df['Selected_AES_Mode'] == '128']['Entropy'].mean(),
    df[df['Selected_AES_Mode'] == '192']['Entropy'].mean(),
    df[df['Selected_AES_Mode'] == '256']['Entropy'].mean(),
    df['Entropy'].mean()
]

bars2 = ax2.bar(modes, entropy_vals, color=['#2E86AB', '#A23B72', '#F18F01', '#6A994E'],
                edgecolor='black', linewidth=0.5)
ax2.set_ylabel('Entropy (bits/byte)', fontsize=12)
ax2.set_title('Ciphertext Entropy by AES Mode', fontsize=14)
ax2.set_ylim(0, 8)
ax2.axhline(y=8.0, color='red', linestyle='--', label='Max (8.0)')
ax2.legend()

for bar, val in zip(bars2, entropy_vals):
    ax2.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.1,
            f'{val:.2f}', ha='center', va='bottom', fontsize=9)

plt.tight_layout()
plt.savefig('fig4_security_metrics.png', bbox_inches='tight')
plt.show()
print("✅ fig4_security_metrics.png saved")

print("\n" + "="*60)
print("ALL GRAPHS GENERATED SUCCESSFULLY")
print("="*60)