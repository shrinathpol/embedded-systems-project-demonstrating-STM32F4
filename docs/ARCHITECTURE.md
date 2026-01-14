# System Architecture

## Overview

This document describes the overall architecture of the STM32 ADC/DMA project, including the module organization, data flow, and interaction patterns.

## Module Organization

```
Application Layer (main.c)
         ↓
┌─────────────────────────────────────────┐
│  Middleware Layer                       │
│  ├── Command Parser                     │
│  ├── Data Logger                        │
│  └── Calibration Manager                │
└─────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────┐
│  Drivers Layer                          │
│  ├── Ring Buffer                        │
│  ├── CRC/Checksum                       │
│  └── Sensors                            │
└─────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────┐
│  Core/HAL Layer                         │
│  ├── ADC Driver                         │
│  ├── DMA Driver                         │
│  ├── Timer Driver                       │
│  └── UART Driver                        │
└─────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────┐
│  Hardware Layer (STM32F411CE)           │
│  ├── Analog Front-End                   │
│  ├── DMA Controller                     │
│  ├── Timers                             │
│  └── UART Peripherals                   │
└─────────────────────────────────────────┘
```

## Data Flow

### ADC Sampling Pipeline

```
┌─────────┐
│ Timer 2 │ (100 Hz)
└────┬────┘
     │ TRGO Event
     ↓
┌─────────────────┐
│ ADC1 Trigger    │
└────┬────────────┘
     │
     ↓
┌────────────────────────┐
│ ADC Conversion         │ (12-bit)
│ PA0 → 0-4095 counts    │
└────┬───────────────────┘
     │
     ↓
┌─────────────────────────┐
│ DMA2 Stream 0           │ (16-bit transfer)
│ ADC→DR → adc_raw_value  │
└────┬────────────────────┘
     │
     ↓
┌─────────────────────────┐
│ DMA Interrupt           │
│ TCIF0 → ISR             │
└────┬────────────────────┘
     │
     ↓
┌─────────────────────────┐
│ Main Loop Processing    │
│ - Convert to voltage    │
│ - Format message        │
│ - Send via UART         │
└────┬────────────────────┘
     │
     ↓
┌─────────────────────────┐
│ UART TX to Host         │
│ 115200 Baud             │
└─────────────────────────┘
```

## Initialization Sequence

```
main()
 ├─ GPIO_Init()        // Configure pins
 ├─ DMA_Init()         // Setup DMA2 Stream 0
 ├─ ADC_Init()         // Configure ADC1
 ├─ UART1_Init()       // Setup UART communication
 ├─ TIM2_Init()        // Configure sampling timer
 ├─ error_init()       // Initialize error handling
 ├─ ring_buffer_init() // Initialize data buffer
 ├─ __enable_irq()     // Enable global interrupts
 └─ TIM2→CR1 |= CEN    // Start timer
```

## Module Responsibilities

### Core Modules (src/core/)

**adc.c**
- ADC1 configuration and initialization
- Voltage conversion (raw counts → mV)
- DMA interrupt handling
- ADC status tracking

**dma.c**
- DMA2 Stream 0 setup
- DMA channel configuration
- Error handling for DMA transfers

**timer.c**
- TIM2 initialization
- Prescaler and period calculation
- Master mode configuration for ADC trigger

**uart.c**
- USART1 initialization
- Baud rate configuration
- TX buffering and management

### Driver Modules (src/drivers/)

**buffer.c**
- Ring buffer (circular buffer) implementation
- Thread-safe write/read operations
- Overflow/underflow detection

**sensors.c**
- Multi-channel sensor support (future)
- Differential measurement support
- Sensor calibration helpers

**crc.c**
- Checksum calculation
- CRC error detection
- Packet validation

### Middleware Modules (src/middleware/)

**command.c**
- UART command parsing
- Command dispatching
- Response formatting

**logger.c**
- Flash memory management
- Data logging to persistent storage
- Log retrieval and formatting

**calibration.c**
- ADC calibration routines
- Offset/gain correction
- Calibration data storage

### Utility Modules (src/utils/)

**error.c**
- Error code definitions
- Error history tracking
- Error reporting and logging

**watchdog.c**
- Watchdog timer setup
- Watchdog kick mechanism
- Timeout handling

**stats.c**
- Min/max/average calculation
- Data trending
- Statistics reporting

## Configuration Management

All configurable parameters are in `include/config.h`:
- Clock frequencies
- ADC parameters
- Buffer sizes
- Feature flags
- Debug settings

## Interrupt Priority

| Priority | Interrupt | Purpose |
|----------|-----------|---------|
| 0 | DMA2_Stream0 | ADC data transfer |
| 1 | TIM2 | Timer overflow (future) |
| 2 | UART1 | Serial communication |

## Memory Layout

```
┌──────────────────────────────┐
│ Code & Constants             │ ~20KB
├──────────────────────────────┤
│ Static Data (adc_raw_value)  │ ~10 bytes
│ Error History                │ ~100 bytes
│ Buffers (ADC, UART, Log)     │ ~6KB
├──────────────────────────────┤
│ Stack                        │ ~2KB
├──────────────────────────────┤
│ Heap                         │ ~1KB
├──────────────────────────────┤
│ FLASH (512KB total)          │
└──────────────────────────────┘
```

## Feature Implementation Timeline

**Phase 1 (Current)**
- Core ADC/DMA/Timer/UART functionality
- Circular buffer for data storage
- Basic error handling

**Phase 2**
- Command interface
- Multi-channel ADC
- CRC/checksum validation
- Statistics calculation

**Phase 3**
- Data logging to flash
- Calibration routines
- Low-power modes
- RTC integration

**Phase 4**
- I2C/SPI interfaces
- Advanced features
- Testing & optimization

---

*Last Updated: January 14, 2026*
