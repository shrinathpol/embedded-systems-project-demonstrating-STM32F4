# STM32 ADC with DMA Project

An embedded systems project demonstrating STM32F4 (BlackPill) microcontroller capabilities with analog-to-digital conversion (ADC), direct memory access (DMA), timer-based sampling, and UART communication.

## Project Overview

This project implements a complete data acquisition system using the STM32F411CE microcontroller on a BlackPill development board. It samples an analog input (potentiometer/sensor) at regular intervals using a timer-triggered ADC with DMA, converts the readings to voltage values, and transmits the results via UART.

## Hardware Configuration

### Hardware Map
- **PA0** - Analog Input (Potentiometer/Sensor)
- **PA9** - UART TX (Connect to RX of USB-TTL converter)
- **PC13** - Onboard LED (Visual heartbeat indicator)

### Board
- **MCU**: STM32F411CE (BlackPill)
- **Clock**: 16 MHz HSI (default)
- **UART Baud Rate**: 115200

## Features

- **Timer-Based Sampling** - TIM2 triggers ADC conversions at regular intervals
- **DMA Transfer** - Direct memory access for efficient data transfer without CPU intervention
- **ADC Conversion** - 12-bit ADC with 3.3V reference
- **Voltage Calculation** - Integer-based math (no floating point) for efficiency
- **UART Communication** - Real-time data logging via serial interface
- **Interrupt-Driven** - Efficient use of hardware interrupts and DMA callbacks

## Folder Structure

-   `src/main.c`: The main application firmware with ADC/DMA/Timer/UART implementations
-   `include/`: Header files
-   `lib/`: Project-specific libraries
-   `test/`: Test files
-   `platformio.ini`: PlatformIO project configuration file

## Getting Started

1.  Clone or open this repository
2.  Open the project in Visual Studio Code with the PlatformIO IDE extension installed
3.  The dependencies should be installed automatically

## Building and Uploading

### Build
```bash
pio run -e blackpill_f411ce
```

### Upload
```bash
pio run -e blackpill_f411ce --target upload
```

Or use the "PlatformIO: Build" and "PlatformIO: Upload" commands in VS Code.

### Monitor Serial Output
```bash
pio device monitor --speed 115200
```

## How It Works

1. **Initialization** - Configures GPIO, DMA, ADC, Timer, and UART
2. **Timer Trigger** - TIM2 generates periodic triggers to start ADC conversions
3. **ADC Sampling** - ADC converts analog input on PA0 to digital value (0-4095)
4. **DMA Transfer** - DMA automatically transfers ADC result to memory and raises an interrupt
5. **Data Processing** - Main loop converts ADC value to voltage and transmits via UART
6. **LED Indicator** - PC13 LED provides visual feedback

## Data Format

ADC values are converted to millivolts using the formula:
```
Voltage(mV) = (ADC_Value × 3300) / 4095
```

Results are transmitted via UART in a human-readable format for monitoring and analysis.

## Configuration

### Timer Sampling Rate
Edit `src/main.c` to modify the timer configuration for different sampling rates.

### UART Settings
- **Baud Rate**: 115200 (configured in main.c)
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None

### Analog Reference
- **Reference Voltage**: 3.3V
- **ADC Resolution**: 12-bit (0-4095 LSBs)

## Debugging

Use ST-Link V2 debugger with PlatformIO:
```bash
pio debug -e blackpill_f411ce
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

## Git Repository Setup

### Initialize Repository (First Time)
```bash
git init
git add .
git commit -m "Initial commit: STM32 ADC/DMA/Timer project"
git remote add origin <your-repository-url>
git branch -M main
git push -u origin main
```

### Clone Repository
```bash
git clone <repository-url>
cd STM32
platformio run --target upload
```

### Common Git Workflows

**Create a feature branch:**
```bash
git checkout -b feature/your-feature-name
git add .
git commit -m "Add feature description"
git push -u origin feature/your-feature-name
```

**Push changes to main:**
```bash
git add .
git commit -m "Commit message describing changes"
git push origin main
```

**Pull latest changes:**
```bash
git pull origin main
```

### `.gitignore` Configuration
The `.gitignore` file automatically excludes:
- PlatformIO build directories (`.pio/`)
- VS Code configuration (`.vscode/`)
- Generated firmware files (`.elf`, `.hex`, `.bin`)
- IDE cache and temporary files
- Python virtual environments

This ensures only source code and essential project files are tracked.

### Git Best Practices for Embedded Projects

1. **Commit frequently** - Save progress after each working feature
2. **Write descriptive commit messages** - Explain what and why, not just what
3. **Branch for features** - Use branches for development, keep main stable
4. **Avoid committing binaries** - Only track source code, not build artifacts
5. **Tag releases** - Mark stable versions with git tags
   ```bash
   git tag -a v1.0 -m "Initial release"
   git push origin v1.0
   ```

### Useful Git Commands

| Command | Purpose |
|---------|---------|
| `git status` | Check modified files |
| `git diff` | View changes before committing |
| `git log` | View commit history |
| `git blame <file>` | Track changes per line |
| `git revert <commit>` | Undo specific commit |
| `git stash` | Save uncommitted changes temporarily |

## License

This project is provided as-is for educational and development purposes.
