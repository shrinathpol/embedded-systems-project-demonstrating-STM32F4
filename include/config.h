#ifndef __CONFIG_H__
#define __CONFIG_H__

/* ============================================
   Board Configuration
   ============================================ */
#define BOARD_STM32F411CE
#define CLOCK_SOURCE_HSI
#define CLOCK_FREQ_MHZ 16
#define SYSCLK_FREQ (16000000UL)

/* ============================================
   ADC Configuration
   ============================================ */
#define ADC_CHANNELS 1                  // Number of ADC channels
#define ADC_SAMPLE_RATE_HZ 100          // Sampling frequency
#define ADC_RESOLUTION 12               // 12-bit resolution
#define ADC_MAX_VALUE ((1 << ADC_RESOLUTION) - 1)  // 4095
#define ADC_REFERENCE_MV 3300           // 3.3V reference

/* ============================================
   Timer Configuration
   ============================================ */
#define TIM2_PRESCALER (1600 - 1)       // Prescale to 10kHz
#define TIM2_PERIOD (100 - 1)           // 100Hz sampling rate

/* ============================================
   UART Configuration
   ============================================ */
#define UART_BAUDRATE 115200
#define UART_BUFFER_SIZE 256
#define UART_TX_TIMEOUT_MS 1000

/* ============================================
   Buffer Configuration
   ============================================ */
#define DATA_BUFFER_SIZE 1024           // ADC data circular buffer
#define UART_RX_BUFFER_SIZE 256
#define LOG_BUFFER_SIZE 4096

/* ============================================
   Feature Flags (Phase 1+)
   ============================================ */
#define ENABLE_WATCHDOG 0               // Watchdog timer
#define ENABLE_ERROR_HANDLING 1         // Error detection
#define ENABLE_LOGGING 0                // Data logging to flash
#define ENABLE_CALIBRATION 0            // ADC calibration
#define ENABLE_STATISTICS 0             // Min/max/avg calculation
#define ENABLE_COMMAND_INTERFACE 0      // UART command parser
#define ENABLE_MULTICHANNEL 0           // Multiple ADC channels

/* ============================================
   Debug Configuration
   ============================================ */
#define DEBUG_UART 1                    // UART debug output
#define DEBUG_LEVEL 2                   // 0=Off, 1=Errors, 2=Info, 3=Verbose

/* ============================================
   Performance Tuning
   ============================================ */
#define DMA_PRIORITY DMA_PRIORITY_HIGH
#define INTERRUPT_PRIORITY 0
#define USE_FLOATING_POINT 0            // Use integer math only

#endif // __CONFIG_H__
