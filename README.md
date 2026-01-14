# 🔧 STM32F4 Data Acquisition System

![Status](https://img.shields.io/badge/Status-Active-green)
![Language](https://img.shields.io/badge/Language-C-blue)
![Platform](https://img.shields.io/badge/Platform-STM32F411CE-red)
![License](https://img.shields.io/badge/License-MIT-green)

> A embedded systems project demonstrating real-time analog data acquisition on STM32F4 microcontroller with DMA-based efficiency and modular architecture.

## 📋 Overview

This project implements a **complete data acquisition system** featuring:

- **Hardware-triggered ADC sampling** at 100 Hz with timer control
- **DMA-based data transfer** for CPU-efficient operation
- **Professional modular architecture** with clear separation of concerns
- **Real-time UART communication** for live data monitoring
- **Comprehensive error handling** and status tracking
- **Circular buffering** for data storage and retrieval

### Key Metrics
| Parameter | Value |
|-----------|-------|
| **Sampling Rate** | 100 Hz |
| **ADC Resolution** | 12-bit (0-4095 counts) |
| **Voltage Range** | 0 - 3.3V |
| **UART Baud** | 115200 bps |
| **CPU Clock** | 16 MHz HSI |

## 🔌 Hardware Configuration

### Pin Assignment
```
┌─────────────────────────────────────────┐
│         STM32F411CE (BlackPill)         │
├─────────────────────────────────────────┤
│ PA0   → Analog Input (ADC Channel 0)    │ ← Sensor/Potentiometer
│ PA9   → UART TX (Serial Output)         │ ← USB-TTL Converter
│ PC13  → LED Output (Status Indicator)   │ ← Visual Feedback
└─────────────────────────────────────────┘
```

### Board Specifications
```
MCU         : STM32F411CE (ARM Cortex-M4)
Flash       : 512 KB
SRAM        : 128 KB  
Clock       : 16 MHz HSI (default, internal)
ADC         : 12-bit resolution, 1 MSPS
DMA         : 2 controllers, 8 channels total
UART        : 6 UART/USART channels
Timers      : Multiple 32-bit & 16-bit timers
```

## ✨ Features & Capabilities

### Core Features
| Feature | Description | Benefit |
|---------|-------------|---------|
| **Timer-Triggered Sampling** | TIM2 @ 100 Hz | Precise, consistent timing |
| **DMA Transfer** | Zero CPU overhead | Efficient data movement |
| **12-bit ADC** | 0-4095 resolution | Good precision |
| **Integer Math** | No floating-point | Faster, smaller code |
| **UART Output** | Real-time streaming | Live data monitoring |
| **Interrupt Driven** | Hardware callbacks | Low latency |

### Advanced Capabilities (Phase 2+)
- 🔄 Multi-channel ADC support
- 💾 Data logging to flash memory
- ⚙️ ADC calibration routines
- 📊 Min/Max/Average statistics
- ⌨️ UART command interface
- 🔍 CRC error detection
- ⏱️ Watchdog timer
- 💤 Low-power modes

## 📁 Project Structure

```
STM32/
│
├── 📂 src/                          # Source code
│   ├── main.c                       # Application entry point
│   ├── 📂 core/                     # Core drivers
│   │   ├── adc.c/h                  # ADC driver
│   │   ├── dma.c/h                  # DMA configuration
│   │   ├── timer.c/h                # Timer control
│   │   └── uart.c/h                 # Serial communication
│   ├── 📂 drivers/                  # Utility drivers
│   │   ├── buffer.c/h               # Ring buffer implementation
│   │   ├── crc.c/h                  # Error detection
│   │   └── sensors.c/h              # Sensor utilities
│   ├── 📂 middleware/               # High-level features
│   │   ├── command.c/h              # Command parser
│   │   ├── logger.c/h               # Data logging
│   │   └── calibration.c/h          # Calibration
│   └── 📂 utils/                    # System utilities
│       ├── error.c/h                # Error handling
│       ├── watchdog.c/h             # Watchdog timer
│       └── stats.c/h                # Statistics
│
├── 📂 include/                      # Header files (mirror structure)
│   ├── config.h                     # Centralized configuration
│   ├── 📂 core/, drivers/, etc.
│
├── 📂 docs/                         # Documentation
│   ├── ARCHITECTURE.md              # System design & diagrams
│   ├── API.md                       # Complete API reference
│   └── EXAMPLES.md                  # Usage examples
│
├── 📂 test/                         # Unit tests
│   ├── test_adc.c
│   ├── test_buffer.c
│   └── test_crc.c
│
├── 📄 README.md                     # This file
├── 📄 STRUCTURE_GUIDE.md            # Folder organization guide
├── 📄 FEATURE_ROADMAP.md            # Development roadmap
├── 📄 IMPLEMENTATION_STATUS.md      # Progress tracking
├── 📄 GIT_WORKFLOW.md               # Git usage guide
├── 📄 platformio.ini                # PlatformIO config
├── 📄 .gitignore                    # Git ignore rules
└── 📄 .gitattributes                # Git attributes

```

### Module Responsibilities
- **Core**: Low-level hardware drivers (ADC, DMA, Timer, UART)
- **Drivers**: Reusable utility modules (Buffer, CRC, Sensors)
- **Middleware**: High-level features (Commands, Logging, Calibration)
- **Utils**: System utilities (Error, Watchdog, Statistics)

## 🚀 Quick Start

### Prerequisites
- ✅ VS Code with PlatformIO extension
- ✅ STM32F411CE BlackPill board
- ✅ ST-Link V2 debugger
- ✅ USB-TTL serial converter (for monitoring)

### Setup & Build

```bash
# 1. Clone repository
git clone https://github.com/shrinathpol/embedded-systems-project-demonstrating-STM32F4.git
cd STM32

# 2. Build project
pio run -e blackpill_f411ce

# 3. Upload to board
pio run -e blackpill_f411ce --target upload

# 4. Monitor serial output
pio device monitor --speed 115200
```

### Expected Output
```
ADC:    0 | Volts: 0.000 V
ADC:  205 | Volts: 0.165 V
ADC:  410 | Volts: 0.330 V
ADC:  615 | Volts: 0.496 V
ADC:  820 | Volts: 0.661 V
ADC: 1024 | Volts: 0.826 V
```

## 🔄 Data Pipeline

```
┌──────────────┐
│  Timer (TIM2)│  ← Oscillates at 100 Hz
└──────┬───────┘
       │ TRGO event
       ▼
┌──────────────────┐
│  ADC1 Trigger    │  ← Rising edge detection
└──────┬───────────┘
       │
       ▼
┌──────────────────────┐
│  ADC Conversion      │  ← 12-bit conversion
│  PA0 → 0-4095       │
└──────┬───────────────┘
       │
       ▼
┌──────────────────────────┐
│  DMA2 Stream 0           │  ← Zero-CPU transfer
│  ADC→DR → RAM           │
└──────┬───────────────────┘
       │ Transfer Complete
       ▼
┌──────────────────────────┐
│  DMA Interrupt Handler   │  ← Set completion flag
│  ISR → Main Loop         │
└──────┬───────────────────┘
       │
       ▼
┌──────────────────────────┐
│  Data Processing         │  ← Convert to voltage
│  Format & Transmit       │
└──────┬───────────────────┘
       │
       ▼
┌──────────────────────────┐
│  UART Output             │  ← Real-time monitor
│  "ADC: xxxx | Volts: x.x"│
└──────────────────────────┘
```

## ⚙️ Configuration & Customization

### Key Settings (`include/config.h`)

```c
/* ADC Configuration */
#define ADC_SAMPLE_RATE_HZ 100      // Change sampling frequency
#define ADC_REFERENCE_MV 3300       // Voltage reference
#define ADC_RESOLUTION 12           // Bit resolution

/* Timer Configuration */  
#define TIM2_PRESCALER (1600 - 1)   // Frequency divider
#define TIM2_PERIOD (100 - 1)       // Determines sample rate

/* UART Configuration */
#define UART_BAUDRATE 115200        // Serial speed
#define UART_BUFFER_SIZE 256        // TX/RX buffer

/* Feature Flags */
#define ENABLE_WATCHDOG 0           // Watchdog timer
#define ENABLE_ERROR_HANDLING 1     // Error detection
#define ENABLE_LOGGING 0            // Flash logging
#define ENABLE_CALIBRATION 0        // ADC calibration
#define ENABLE_STATISTICS 0         // Stats calculation
```

### Voltage Conversion Formula

```
Voltage(mV) = (ADC_Raw × Reference_mV) / Max_Counts
            = (ADC_Raw × 3300) / 4095
            = (ADC_Raw * 806) / 1000  // Optimized integer math
```

### Frequency Calculation

```
Clock Frequency    = 16 MHz (HSI)
After Prescale     = 16 MHz / 1600 = 10 kHz
Sample Rate        = 10 kHz / 100 = 100 Hz
Sample Period      = 10 ms
```

## Hardware Requirements

- STM32F411CE BlackPill board
- ST-Link V2 debugger/programmer
- USB-TTL serial converter (for UART communication)
- 3.3V input source for analog testing
- USB power supply or external power

## References

- [STM32F4 Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- [BlackPill Documentation](https://github.com/WeActStudio/BluePill)
- [PlatformIO Documentation](https://docs.platformio.org/)

## Code Explanation

### Global Variables
```c
volatile uint16_t adc_raw_value = 0;           // Stores ADC conversion result
volatile uint8_t dma_transfer_complete = 0;    // Flag set by DMA interrupt
```
- Marked `volatile` to prevent compiler optimization, as they're modified by hardware interrupts

### Main Function Flow
1. **Initialize Peripherals** - Configure GPIO, DMA, ADC, UART, and Timer
2. **Enable Interrupts** - Allow DMA to trigger interrupts
3. **Start Timer** - Begins the conversion cycle at 100 Hz
4. **Main Loop** - Waits for DMA completion flag, then processes and transmits data

### Core Initialization Functions

#### `GPIO_Init()`
- Enables GPIO port clocks for Port A and C
- PA0: Configured as analog input (MODER = 11) for ADC channel 0
- PA9: Configured as alternate function (MODER = 10) with AF7 for USART1 TX
- PC13: Configured as output (MODER = 01) for LED control

#### `TIM2_Init()`
- Prescaler: 1600 - 1 (divides 16 MHz → 10 kHz)
- Auto-reload: 100 - 1 (divides 10 kHz → 100 Hz sampling)
- Master Mode: Set to send TRGO signal on timer update event
- Generates a trigger pulse every 10 ms for ADC conversions

#### `ADC_Init()`
- Enables ADC1 clock and configures for external triggering
- EXTEN = 01: Rising edge trigger detection
- EXTSEL = 0110: Selects TIM2 TRGO as trigger source
- DMA and DDS enabled for continuous conversions
- Single channel (PA0) in conversion sequence

#### `DMA_Init()`
- Enables DMA2 clock (ADC1 connects to DMA2 Stream 0)
- Configured for:
  - 16-bit peripheral and memory sizes
  - Circular mode (auto-repeats after transfer)
  - No memory increment (overwrites same variable)
  - Transfer complete interrupt enabled
- Transfers ADC data register → `adc_raw_value` variable
- Generates interrupt on completion

#### `UART1_Init()`
- Configures USART1 for 115200 baud @ 16 MHz HSI
- BRR = 0x8B: Baud rate register value
- Enables transmit and UART module

#### `UART1_SendString()`
- Polls TX empty flag before sending each character
- Transmits string data byte-by-byte
- Blocks until all data sent

#### `DMA2_Stream0_IRQHandler()`
- Interrupt handler called when DMA completes data transfer
- Clears the transfer complete flag (TCIF0)
- Sets software flag to notify main loop

### Main Loop Operation

```c
if (dma_transfer_complete) {
    dma_transfer_complete = 0;                           // Clear flag
    uint32_t voltage_mv = (adc_raw_value * 3300) / 4095; // Convert to mV
    uint32_t val_whole = voltage_mv / 1000;             // Whole volts
    uint32_t val_dec = voltage_mv % 1000;               // Decimal portion
    
    sprintf(msg_buffer, "ADC: %4d | Volts: %lu.%03lu V\r\n", ...);
    UART1_SendString(msg_buffer);
    GPIOC->ODR ^= (1 << 13);                            // Toggle LED
}
```

### Data Flow
```
Timer (100 Hz) → ADC Trigger → ADC Conversion → DMA Transfer → Interrupt
                                                      ↓
                                                 Set Flag
                                                      ↓
                                              Main Loop Detects
                                                      ↓
                                         Convert & Transmit via UART
                                                      ↓
                                                 Toggle LED
```

### Register Bit Definitions

**GPIO MODER (Mode Register)**
- 00 = Input
- 01 = Output  
- 10 = Alternate Function
- 11 = Analog

**ADC CR2 Bits**
- Bit 28: EXTEN (External trigger enable)
- Bits 24-27: EXTSEL (External trigger selection)
- DMA: Enable DMA mode
- DDS: DMA disable selection

**DMA Stream CR (Control Register)**
- Bit 4: TCIE (Transfer Complete Interrupt Enable)
- Bit 8: PSIZE (Peripheral size = 16-bit)
- Bit 11: CIRC (Circular mode)
- Bit 13: MSIZE (Memory size = 16-bit)

## References

- [STM32F4 Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- [BlackPill Documentation](https://github.com/WeActStudio/BluePill)
- [PlatformIO Documentation](https://docs.platformio.org/)
