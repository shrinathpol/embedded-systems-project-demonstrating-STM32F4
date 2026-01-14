# ⚙️ Configuration & Customization Guide

## Configuration Overview

All project settings are centralized in [`include/config.h`](../include/config.h) for easy management and modification.

## Configuration Sections

### 🔷 Board Configuration

```c
#define BOARD_STM32F411CE              // Target MCU
#define CLOCK_SOURCE_HSI               // Internal 16 MHz oscillator
#define CLOCK_FREQ_MHZ 16              // Clock frequency
#define SYSCLK_FREQ (16000000UL)       // System clock in Hz
```

**When to modify:**
- Switching to different board/MCU
- Using external oscillator (need clock code changes)
- Running at different clock speed

---

### 📊 ADC Configuration

```c
#define ADC_CHANNELS 1                 // Number of ADC channels
#define ADC_SAMPLE_RATE_HZ 100         // Sampling frequency (Hz)
#define ADC_RESOLUTION 12              // Bit resolution (10 or 12)
#define ADC_MAX_VALUE 4095             // Max count (2^12 - 1)
#define ADC_REFERENCE_MV 3300          // Reference voltage (mV)
```

**Quick adjustments:**

| Use Case | Modification |
|----------|--------------|
| **Faster sampling** | `ADC_SAMPLE_RATE_HZ` → 200, 500, 1000 |
| **Different voltage** | `ADC_REFERENCE_MV` → 5000 for 5V |
| **Less resolution** | `ADC_RESOLUTION` → 10, `MAX_VALUE` → 1023 |
| **Multiple channels** | `ADC_CHANNELS` → 2, 4, 8 (Phase 2) |

**Formula for changing sample rate:**
```
Prescaler = Clock / (Prescaler * ARR)
16 MHz / (1600 * 100) = 100 Hz

For 1 kHz:
16 MHz / (160 * 100) or 16 MHz / (1600 * 10)
```

---

### ⏱️ Timer Configuration

```c
#define TIM2_PRESCALER (1600 - 1)      // Divider: 16MHz → 10kHz
#define TIM2_PERIOD (100 - 1)          // Counter: 10kHz → 100Hz
```

**Calculation:**
```
Output_Frequency = Clock_Freq / ((PSC + 1) * (ARR + 1))
100 Hz = 16,000,000 / ((1600) * (100))
       = 16,000,000 / 160,000
       = 100 ✓
```

**Common frequencies:**

| Frequency | PSC | ARR | Notes |
|-----------|-----|-----|-------|
| 1 Hz | 15999 | 1000 | Very slow, testing |
| 10 Hz | 1599 | 1000 | Moderate |
| 100 Hz | 1599 | 100 | **Current** |
| 1 kHz | 159 | 1000 | Fast |
| 10 kHz | 15 | 1000 | Very fast |

---

### 🔌 UART Configuration

```c
#define UART_BAUDRATE 115200           // Serial speed (baud)
#define UART_BUFFER_SIZE 256           // TX/RX buffer
#define UART_TX_TIMEOUT_MS 1000        // Transmission timeout
```

**Baud rate selection:**

| Speed | Use Case | Notes |
|-------|----------|-------|
| 9600 | Legacy systems | Slow, reliable |
| 19200 | Moderate speed | Good balance |
| 57600 | Fast data | Common |
| **115200** | **Fast data** | **Recommended** |
| 230400 | Very fast | May need tuning |

---

### 💾 Buffer Configuration

```c
#define DATA_BUFFER_SIZE 1024          // ADC circular buffer
#define UART_RX_BUFFER_SIZE 256        // UART RX buffer
#define LOG_BUFFER_SIZE 4096           // Data logging buffer
```

**Memory implications:**
```
Total RAM = (DATA_BUFFER + UART_RX + LOG) * element_size
          = (1024 + 256 + 4096) * 2 bytes (uint16_t)
          = ~10.5 KB (out of 128 KB available)
```

**Adjustment strategy:**

| Scenario | Buffer Size |
|----------|-------------|
| Memory constrained | 256, 128, 512 |
| Standard operation | **1024, 256, 4096** |
| Data heavy logging | 4096, 512, 16384 |

---

### ✨ Feature Flags

```c
/* Phase 1 - Core */
#define ENABLE_ERROR_HANDLING 1        // Error detection ✅
#define ENABLE_WATCHDOG 0              // Watchdog timer ❌

/* Phase 2 - Enhanced */
#define ENABLE_LOGGING 0               // Flash data logging ❌
#define ENABLE_CALIBRATION 0           // ADC calibration ❌
#define ENABLE_STATISTICS 0            // Min/max/avg ❌
#define ENABLE_COMMAND_INTERFACE 0     // UART commands ❌
#define ENABLE_MULTICHANNEL 0          // Multi-channel ADC ❌

/* Phase 3+ - Advanced */
#define ENABLE_RTC 0                   // Real-time clock ❌
#define ENABLE_LOW_POWER 0             // Sleep modes ❌
#define ENABLE_I2C 0                   // I2C interface ❌
```

**To enable a feature:**
```c
#define ENABLE_WATCHDOG 1              // Change 0 to 1
```

---

### 🐛 Debug Configuration

```c
#define DEBUG_UART 1                   // Enable UART debug output
#define DEBUG_LEVEL 2                  // 0=Off, 1=Errors, 2=Info, 3=Verbose
```

**Debug levels:**
| Level | Output | Use Case |
|-------|--------|----------|
| 0 | None | Production |
| 1 | Errors only | Testing |
| 2 | **Info messages** | **Development** |
| 3 | Verbose (all) | Debugging |

---

### ⚡ Performance Tuning

```c
#define USE_FLOATING_POINT 0           // 0=Integer math (fast)
#define DMA_PRIORITY DMA_PRIORITY_HIGH // DMA priority level
#define INTERRUPT_PRIORITY 0           // Interrupt nesting
```

**Performance vs. Size:**
```
USE_FLOATING_POINT = 0  →  Faster, smaller code
USE_FLOATING_POINT = 1  →  More precise, larger code
```

---

## Common Configuration Scenarios

### 🔹 Scenario 1: High-Speed Sensor

**Goal:** Sample at 1 kHz with high resolution
```c
#define ADC_SAMPLE_RATE_HZ 1000
#define ADC_RESOLUTION 12
#define TIM2_PRESCALER (15)
#define TIM2_PERIOD (1000)
#define UART_BAUDRATE 230400          // Faster serial
#define DEBUG_LEVEL 0                 // Disable debug output
```

### 🔹 Scenario 2: Low-Power Monitoring

**Goal:** Minimal power consumption
```c
#define ADC_SAMPLE_RATE_HZ 1
#define ENABLE_WATCHDOG 1
#define ENABLE_LOW_POWER 1
#define DEBUG_LEVEL 0
#define UART_BUFFER_SIZE 64           // Smaller buffer
#define DATA_BUFFER_SIZE 256
```

### 🔹 Scenario 3: Data Logging System

**Goal:** Store data and analyze later
```c
#define ADC_SAMPLE_RATE_HZ 100        // Standard rate
#define ENABLE_LOGGING 1               // Enable logging
#define ENABLE_STATISTICS 1            // Track stats
#define LOG_BUFFER_SIZE 16384         // Large log buffer
#define ENABLE_COMMAND_INTERFACE 1    // Control via UART
```

### 🔹 Scenario 4: Educational/Development

**Goal:** Easy to understand and modify
```c
#define ADC_SAMPLE_RATE_HZ 100
#define DEBUG_LEVEL 2                 // Full debug info
#define ENABLE_ERROR_HANDLING 1       // Error tracking
#define UART_BAUDRATE 115200
/* All features disabled for clarity */
```

---

## Calculation Helper

### Timer Frequency Calculator

```c
// To calculate timer prescaler and period:
// Clock frequency: 16 MHz
// Desired output: 100 Hz
// 
// Step 1: Choose intermediate frequency
//   16 MHz / 1600 = 10 kHz (PSC = 1599)
//
// Step 2: Calculate period
//   10 kHz / 100 Hz = 100 (ARR = 99)
//
// Formula: freq = Clock / ((PSC+1) * (ARR+1))

#define PSC_VALUE 1599   // Prescaler
#define ARR_VALUE 99     // Auto-reload
// Result: 16,000,000 / (1600 * 100) = 100 Hz ✓
```

### ADC Voltage Calculator

```c
// Raw ADC value: 2048 (out of 4095)
// Reference voltage: 3300 mV
//
// Voltage_mV = (RAW * REF) / MAX
//            = (2048 * 3300) / 4095
//            = 6,758,400 / 4095
//            = 1650 mV
//            = 1.650 V ✓
```

---

## Configuration Validation

### Pre-build Checklist

```c
// ✅ Verify ADC configuration
static_assert(ADC_RESOLUTION == 10 || ADC_RESOLUTION == 12, "ADC resolution must be 10 or 12 bits");

// ✅ Verify clock configuration  
static_assert(CLOCK_FREQ_MHZ == 16, "Only 16MHz HSI supported currently");

// ✅ Verify buffer sizes
static_assert(DATA_BUFFER_SIZE >= 256, "Buffer too small");
static_assert(DATA_BUFFER_SIZE <= 65536, "Buffer too large");

// ✅ Verify debug level
static_assert(DEBUG_LEVEL >= 0 && DEBUG_LEVEL <= 3, "Invalid debug level");
```

---

## Environment-Specific Configs

### Development Environment

```c
// .vscode/settings.json preprocessor symbols:
-DDEBUG_BUILD
-DENABLE_ERROR_HANDLING=1
-DDEBUG_LEVEL=2
```

### Production Environment

```c
// platformio.ini:
build_flags =
    -DRELEASE_BUILD
    -DDEBUG_LEVEL=0
    -DENABLE_ERROR_HANDLING=1
```

---

