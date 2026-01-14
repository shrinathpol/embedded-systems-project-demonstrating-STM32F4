# API Reference

## Core APIs

### ADC Module (`include/core/adc.h`)

#### `adc_status_t adc_init(void)`
Initialize ADC1 with external triggering from TIM2.

**Parameters:** None  
**Returns:** `ADC_STATUS_OK` on success, error code otherwise  
**Example:**
```c
if (adc_init() != ADC_STATUS_OK) {
    error_report(ERROR_ADC_FAILED, 2, "ADC init failed");
}
```

#### `adc_status_t adc_get_reading(adc_reading_t *reading)`
Get the latest ADC conversion result.

**Parameters:**
- `reading`: Pointer to `adc_reading_t` structure

**Returns:** `ADC_STATUS_OK` if data available, `ADC_STATUS_NOT_READY` if conversion pending

**Example:**
```c
adc_reading_t reading;
if (adc_get_reading(&reading) == ADC_STATUS_OK) {
    printf("ADC: %d, Voltage: %lu.%03lu V\r\n", 
           reading.raw_value, 
           reading.voltage_whole, 
           reading.voltage_decimal);
}
```

#### `uint32_t adc_raw_to_voltage_mv(uint16_t raw_value)`
Convert raw ADC counts to voltage in millivolts.

**Parameters:**
- `raw_value`: Raw ADC count (0-4095)

**Returns:** Voltage in millivolts

**Example:**
```c
uint32_t voltage_mv = adc_raw_to_voltage_mv(2048);  // ~1650 mV (3.3V reference)
```

---

## Driver APIs

### Ring Buffer (`include/drivers/buffer.h`)

#### `bool ring_buffer_init(ring_buffer_t *rb, uint16_t *buffer, uint16_t size)`
Initialize a ring buffer.

**Parameters:**
- `rb`: Pointer to ring buffer structure
- `buffer`: Pointer to data buffer
- `size`: Buffer size in elements

**Returns:** `true` on success

**Example:**
```c
static uint16_t data_buffer[1024];
ring_buffer_t adc_buffer;
ring_buffer_init(&adc_buffer, data_buffer, 1024);
```

#### `bool ring_buffer_write(ring_buffer_t *rb, uint16_t data)`
Add data to buffer (write).

**Parameters:**
- `rb`: Pointer to ring buffer
- `data`: Data to write

**Returns:** `true` if successful

**Example:**
```c
if (!ring_buffer_write(&adc_buffer, adc_reading.raw_value)) {
    error_report(ERROR_BUFFER_OVERFLOW, 1, "ADC buffer full");
}
```

#### `bool ring_buffer_read(ring_buffer_t *rb, uint16_t *data)`
Remove and read data from buffer.

**Parameters:**
- `rb`: Pointer to ring buffer
- `data`: Pointer to read location

**Returns:** `true` if data available and read

**Example:**
```c
uint16_t value;
while (ring_buffer_read(&adc_buffer, &value)) {
    printf("Buffered value: %d\r\n", value);
}
```

#### `uint16_t ring_buffer_count(ring_buffer_t *rb)`
Get number of elements in buffer.

**Parameters:**
- `rb`: Pointer to ring buffer

**Returns:** Element count

**Example:**
```c
if (ring_buffer_count(&adc_buffer) > 100) {
    // Buffer has accumulated significant data
}
```

---

## Utility APIs

### Error Handling (`include/utils/error.h`)

#### `void error_init(void)`
Initialize error handling system.

**Parameters:** None  
**Returns:** None

**Example:**
```c
error_init();
```

#### `void error_report(error_code_t code, uint8_t severity, const char *message)`
Report an error.

**Parameters:**
- `code`: Error code (see `error_code_t` enum)
- `severity`: 0=Info, 1=Warning, 2=Error, 3=Critical
- `message`: Error message string

**Returns:** None

**Example:**
```c
if (uart_transmit_failed) {
    error_report(ERROR_UART_FAILED, 2, "UART TX timeout");
}
```

#### `error_t error_get_last(void)`
Get last reported error.

**Parameters:** None  
**Returns:** `error_t` structure with error details

**Example:**
```c
error_t last_error = error_get_last();
printf("Last error: %s (code: 0x%02X)\r\n", 
       last_error.message, last_error.code);
```

#### `bool error_is_critical(void)`
Check if a critical error has occurred.

**Parameters:** None  
**Returns:** `true` if critical error present

**Example:**
```c
if (error_is_critical()) {
    // Trigger system recovery or shutdown
    system_reset();
}
```

---

## Data Structures

### `adc_reading_t`
Contains a single ADC conversion result.

```c
typedef struct {
    uint16_t raw_value;        // Raw ADC count (0-4095)
    uint32_t voltage_mv;       // Voltage in millivolts
    uint32_t voltage_whole;    // Whole volts (V)
    uint32_t voltage_decimal;  // Decimal portion (mV)
    uint64_t timestamp_ms;     // Timestamp
    adc_status_t status;       // Conversion status
} adc_reading_t;
```

### `ring_buffer_t`
Circular buffer for data storage.

```c
typedef struct {
    uint16_t *buffer;          // Data buffer pointer
    uint16_t head;             // Write pointer
    uint16_t tail;             // Read pointer
    uint16_t size;             // Buffer size
    uint16_t count;            // Element count
    bool full;                 // Full flag
} ring_buffer_t;
```

### `error_t`
Error information structure.

```c
typedef struct {
    error_code_t code;         // Error code
    uint32_t timestamp_ms;     // When error occurred
    uint8_t severity;          // 0=Info, 1=Warn, 2=Error, 3=Critical
    const char *message;       // Error message
} error_t;
```

---

## Enumerations

### `adc_status_t`
ADC operation status codes:
- `ADC_STATUS_OK` - Operation successful
- `ADC_STATUS_ERROR` - General error
- `ADC_STATUS_NOT_READY` - ADC not ready or no data available
- `ADC_STATUS_TIMEOUT` - Operation timeout

### `error_code_t`
System error codes:
- `ERROR_NONE` - No error
- `ERROR_ADC_FAILED` - ADC conversion failed
- `ERROR_DMA_FAILED` - DMA transfer failed
- `ERROR_UART_FAILED` - UART communication failed
- `ERROR_TIMER_FAILED` - Timer failed
- `ERROR_BUFFER_OVERFLOW` - Buffer overflow
- `ERROR_BUFFER_UNDERFLOW` - Buffer underflow
- `ERROR_INVALID_PARAM` - Invalid parameter
- `ERROR_TIMEOUT` - Operation timeout

---

## Usage Examples

### Basic ADC Reading Loop

```c
#include "core/adc.h"
#include "utils/error.h"

int main(void) {
    // Initialize
    error_init();
    if (adc_init() != ADC_STATUS_OK) {
        error_report(ERROR_ADC_FAILED, 3, "ADC init failed");
        return 1;
    }

    adc_start();

    // Main loop
    while (1) {
        adc_reading_t reading;
        
        if (adc_get_reading(&reading) == ADC_STATUS_OK) {
            printf("ADC: %4d | Voltage: %lu.%03lu V\r\n",
                   reading.raw_value,
                   reading.voltage_whole,
                   reading.voltage_decimal);
        }
    }

    return 0;
}
```

### Buffered Reading

```c
#include "drivers/buffer.h"

static uint16_t adc_data[1024];
ring_buffer_t adc_buffer;

// In init
ring_buffer_init(&adc_buffer, adc_data, 1024);

// In main loop
if (adc_get_reading(&reading) == ADC_STATUS_OK) {
    ring_buffer_write(&adc_buffer, reading.raw_value);
}

// Read buffered data
uint16_t value;
if (ring_buffer_read(&adc_buffer, &value)) {
    // Process buffered value
}
```

### Error Handling

```c
if (error_is_critical()) {
    // Log critical error
    error_t err = error_get_last();
    printf("CRITICAL: %s\r\n", err.message);
    
    // System recovery
    error_clear();
    // Re-initialize systems as needed
}
```

---
