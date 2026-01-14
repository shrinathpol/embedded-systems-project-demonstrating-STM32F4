# Project Structure Guide

## Recommended Project Organization

```
STM32/
├── src/
│   ├── main.c                 # Main application entry point
│   ├── core/
│   │   ├── adc.c             # ADC initialization and handling
│   │   ├── dma.c             # DMA configuration
│   │   ├── timer.c           # Timer setup and control
│   │   └── uart.c            # UART communication
│   ├── drivers/
│   │   ├── sensors.c         # Sensor reading utilities
│   │   ├── buffer.c          # Circular buffer implementation
│   │   └── crc.c             # Checksum/CRC calculation
│   ├── middleware/
│   │   ├── command.c         # Command parser and handler
│   │   ├── logger.c          # Data logging functionality
│   │   └── calibration.c     # Calibration routines
│   └── utils/
│       ├── error.c           # Error handling
│       ├── watchdog.c        # Watchdog timer
│       └── stats.c           # Statistics calculation
│
├── include/
│   ├── config.h              # Project configuration
│   ├── core/
│   │   ├── adc.h
│   │   ├── dma.h
│   │   ├── timer.h
│   │   └── uart.h
│   ├── drivers/
│   │   ├── sensors.h
│   │   ├── buffer.h
│   │   └── crc.h
│   ├── middleware/
│   │   ├── command.h
│   │   ├── logger.h
│   │   └── calibration.h
│   └── utils/
│       ├── error.h
│       ├── watchdog.h
│       └── stats.h
│
├── lib/
│   └── README
│
├── test/
│   ├── test_adc.c
│   ├── test_buffer.c
│   ├── test_crc.c
│   └── test_command.c
│
├── docs/
│   ├── ARCHITECTURE.md        # System architecture
│   ├── API.md                 # API documentation
│   └── EXAMPLES.md            # Usage examples
│
├── platformio.ini
├── README.md
├── FEATURE_ROADMAP.md
├── GIT_WORKFLOW.md
└── .gitignore
```

## Module Descriptions

### Core Modules (`src/core/`)
Essential hardware drivers and initialization:
- **adc.c** - ADC configuration and data acquisition
- **dma.c** - DMA channel setup and management
- **timer.c** - Timer trigger generation
- **uart.c** - Serial communication

### Driver Modules (`src/drivers/`)
Utility drivers for data handling:
- **sensors.c** - Sensor data reading and conversion
- **buffer.c** - Ring/circular buffer for data storage
- **crc.c** - Checksum and error detection

### Middleware Modules (`src/middleware/`)
High-level functionality:
- **command.c** - UART command parser and dispatcher
- **logger.c** - Data logging and storage
- **calibration.c** - ADC calibration routines

### Utility Modules (`src/utils/`)
System utilities:
- **error.c** - Error handling and recovery
- **watchdog.c** - Watchdog timer management
- **stats.c** - Data statistics (min/max/avg)

## Implementation Phase

### Phase 1: Core Structure (Week 1)
1. Create folder structure
2. Implement circular buffer (drivers/)
3. Add error handling (utils/)
4. Add watchdog timer (utils/)

### Phase 2: Enhanced Features (Week 2)
1. Command interface (middleware/)
2. Multi-channel ADC (core/)
3. CRC validation (drivers/)
4. Statistics calculation (utils/)

### Phase 3: Advanced Features (Week 3)
1. Data logging (middleware/)
2. Calibration routines (middleware/)
3. Low-power modes (core/)
4. RTC support

### Phase 4: Testing & Polish (Week 4)
1. Unit tests (test/)
2. Integration tests
3. Documentation
4. Performance optimization

## Module Dependencies

```
main.c
  ├── core/uart.c
  ├── core/adc.c
  ├── core/dma.c
  ├── core/timer.c
  ├── drivers/buffer.c
  ├── drivers/sensors.c
  ├── middleware/command.c
  ├── middleware/logger.c
  └── utils/error.c
```

## Compilation Strategy

### Step 1: Build Core Modules
```bash
gcc -c src/core/*.c -Iinclude/
```

### Step 2: Build Driver Modules
```bash
gcc -c src/drivers/*.c -Iinclude/
```

### Step 3: Build Middleware
```bash
gcc -c src/middleware/*.c -Iinclude/
```

### Step 4: Build Utilities
```bash
gcc -c src/utils/*.c -Iinclude/
```

### Step 5: Link Everything
```bash
gcc *.o -o firmware.elf
```

## Header File Template

Each module should have a corresponding header:

```c
#ifndef __MODULE_H__
#define __MODULE_H__

#include <stdint.h>
#include <stdbool.h>

/* Type Definitions */
typedef struct {
    // module-specific types
} module_t;

/* Function Declarations */
void module_init(void);
void module_process(void);
bool module_is_ready(void);

#endif // __MODULE_H__
```

## Source File Template

```c
#include "module.h"
#include "../config.h"

/* Private Variables */
static module_t module_data = {0};

/* Private Functions */
static void module_interrupt_handler(void);

/* Public Functions */
void module_init(void) {
    // Implementation
}

void module_process(void) {
    // Implementation
}

bool module_is_ready(void) {
    return module_data.is_ready;
}
```

## Configuration Management

Create `include/config.h` for all project settings:

```c
#ifndef __CONFIG_H__
#define __CONFIG_H__

/* Board Configuration */
#define BOARD_STM32F411CE
#define CLOCK_SOURCE_HSI
#define CLOCK_FREQ_MHZ 16

/* ADC Configuration */
#define ADC_CHANNELS 4
#define ADC_SAMPLE_RATE 100  // Hz
#define ADC_RESOLUTION 12

/* UART Configuration */
#define UART_BAUDRATE 115200
#define UART_BUFFER_SIZE 256

/* Buffer Configuration */
#define DATA_BUFFER_SIZE 1024
#define LOG_BUFFER_SIZE 4096

/* Feature Flags */
#define ENABLE_WATCHDOG 1
#define ENABLE_LOGGING 1
#define ENABLE_CALIBRATION 1
#define ENABLE_STATISTICS 1

#endif // __CONFIG_H__
```

## Build Configuration (platformio.ini)

```ini
[env:blackpill_f411ce]
platform = ststm32
board = blackpill_f411ce
framework = stm32cube
monitor_speed = 115200
upload_protocol = stlink

build_flags =
    -DDEBUG
    -Wl,-u,_printf_float
    -std=c99
    -Wall
    -Wextra

lib_deps =

src_filter =
    +<src/>
    +<include/>

extra_scripts =
    pre:scripts/prebuild.py
    post:scripts/postbuild.py
```

## Version Control Strategy

Create branches for each feature/phase:

```bash
# Feature branches
git checkout -b feature/circular-buffer
git checkout -b feature/error-handling
git checkout -b feature/command-interface
git checkout -b feature/data-logging

# Release branches
git checkout -b release/v0.1-alpha
git checkout -b release/v0.2-beta
```

## Documentation Strategy

### docs/ARCHITECTURE.md
- System design overview
- Module interactions
- Data flow diagrams

### docs/API.md
- Function signatures
- Parameter descriptions
- Return values and error codes

### docs/EXAMPLES.md
- Usage examples
- Code snippets
- Integration patterns

---

**Start with Phase 1 to establish a solid foundation, then build up features incrementally.**
