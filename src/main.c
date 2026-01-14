/**
 * =============================================================================
 * STM32F4 Data Acquisition System - Main Application
 * =============================================================================
 * 
 * This bare-metal CMSIS implementation demonstrates:
 * - Hardware timer configured for 100 Hz trigger
 * - ADC triggered by timer overflow events
 * - DMA transfers ADC results to memory buffer
 * - UART sends results at 115200 baud via serial terminal
 * 
 * Hardware Configuration:
 * - PA0: Analog Input (ADC Channel 0) - Connect potentiometer or sensor here
 * - PA9: UART TX (USB-TTL or Serial Adapter)
 * - PC13: LED Output (Status indicator)
 * 
 * System Flow:
 * 1. Timer fires at 100 Hz
 * 2. Timer TRGO triggers ADC conversion
 * 3. ADC converts PA0 analog input
 * 4. DMA transfers result to buffer
 * 5. DMA interrupt signals data ready
 * 6. Main loop sends result via UART
 * 7. Result displayed on serial terminal
 * 
 * Expected Output (Putty/Serial Terminal):
 * ADC: 1234 | Voltage: 1.005 V
 * ADC: 1245 | Voltage: 1.014 V
 * ADC: 1200 | Voltage: 0.977 V
 * ...
 * 
 * =============================================================================
 */

#include "stm32f4xx.h"
#include "config.h"
#include "core/timer.h"
#include "core/adc.h"
#include "core/dma.h"
#include "core/uart.h"
#include <stdio.h>

/* ============================================
   Global Variables
   ============================================ */

// DMA buffer for ADC conversion results
// Circular buffer - DMA writes here, main reads from here
volatile uint16_t adc_buffer[1] = {0};

// Status LED counter
static volatile uint32_t sample_count = 0;
static volatile uint32_t led_toggle_count = 0;

/* ============================================
   Function Declarations
   ============================================ */

void system_init(void);
void gpio_init(void);
void print_welcome_message(void);
void process_adc_sample(uint16_t raw_value);

/**
 * @brief Main Application Entry Point
 */
int main(void) {
    // Initialize system peripherals
    system_init();
    
    // Print welcome message to serial terminal
    print_welcome_message();
    
    // Main event loop
    while (1) {
        // Check if DMA transfer is complete (new ADC sample available)
        if (dma_is_transfer_complete()) {
            // Clear the DMA complete flag
            dma_clear_transfer_complete_flag();
            
            // Process the ADC sample
            process_adc_sample(adc_buffer[0]);
            
            // Toggle LED for visual feedback
            led_toggle_count++;
            if (led_toggle_count >= 10) {  // Toggle every 10 samples (at 100Hz = every 100ms)
                GPIOC->ODR ^= (1 << 13);   // Toggle PC13 LED
                led_toggle_count = 0;
            }
        }
    }
    
    return 0;
}

/**
 * @brief Initialize all system peripherals
 * 
 * Initialization order:
 * 1. UART (for debug output)
 * 2. GPIO (for analog input and LED)
 * 3. DMA (for ADC data transfer)
 * 4. ADC (for analog input)
 * 5. Timer (starts trigger sequence)
 * 6. Enable interrupts
 */
void system_init(void) {
    // Initialize UART first so we can see debug messages
    uart_init();
    
    // Initialize GPIO for analog input (PA0) and status LED (PC13)
    gpio_init();
    
    // Initialize DMA for ADC data transfer
    dma_init();
    
    // Configure DMA buffer address for ADC data
    DMA2_Stream0->M0AR = (uint32_t)&adc_buffer[0];
    dma_enable();
    
    // Initialize ADC with timer trigger
    adc_init();
    
    // Initialize timer for 100Hz trigger output
    timer_init();
    
    // Enable global interrupts
    __enable_irq();
    
    // Start the timer (this triggers ADC conversions)
    timer_start();
}

/**
 * @brief Initialize GPIO pins
 * 
 * PA0:  Analog input (ADC Channel 0)
 * PA9:  UART TX (already configured in uart_init)
 * PC13: LED output (status indicator)
 */
void gpio_init(void) {
    // Enable GPIO clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   // Enable GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;   // Enable GPIOC
    
    // Configure PA0 as analog input
    // MODER[1:0] = 11 (analog mode)
    GPIOA->MODER &= ~(3U << (0 * 2));      // Clear bits
    GPIOA->MODER |= (3U << (0 * 2));       // Analog mode
    
    // Configure PC13 as output (LED)
    // MODER[27:26] = 01 (output mode)
    GPIOC->MODER &= ~(3U << (13 * 2));     // Clear bits
    GPIOC->MODER |= (1U << (13 * 2));      // Output mode
    
    // Set LED initial state (ON)
    GPIOC->ODR |= (1 << 13);
}

/**
 * @brief Print system initialization message to serial terminal
 */
void print_welcome_message(void) {
    uart_send_string("\r\n");
    uart_send_string("========================================\r\n");
    uart_send_string("STM32F411CE Data Acquisition System\r\n");
    uart_send_string("100 Hz Timer-Triggered ADC with DMA\r\n");
    uart_send_string("========================================\r\n");
    uart_send_string("Configuration:\r\n");
    uart_send_string("  Timer Frequency: 100 Hz\r\n");
    uart_send_string("  ADC Channel: 0 (PA0)\r\n");
    uart_send_string("  ADC Resolution: 12-bit (0-4095)\r\n");
    uart_send_string("  Reference Voltage: 3.3V\r\n");
    uart_send_string("  UART Baud Rate: 115200 bps\r\n");
    uart_send_string("  DMA Mode: Circular, Memory Increment\r\n");
    uart_send_string("========================================\r\n");
    uart_send_string("System Ready. Waiting for ADC samples...\r\n");
    uart_send_string("Monitoring ADC Channel 0 (PA0):\r\n\r\n");
}

/**
 * @brief Process ADC sample and send via UART
 * 
 * Converts raw ADC value to voltage and sends formatted
 * data to serial terminal
 * 
 * @param raw_value Raw 12-bit ADC value (0-4095)
 */
void process_adc_sample(uint16_t raw_value) {
    static char uart_buffer[64];
    
    // Increment sample counter
    sample_count++;
    
    // Convert raw ADC value to voltage (mV)
    // Voltage(mV) = (ADC_Value × Reference) / Max_Count
    // = (raw_value × 3300) / 4095
    uint32_t voltage_mv = (raw_value * 3300) / 4095;
    
    // Split voltage into whole and decimal parts
    uint32_t voltage_whole = voltage_mv / 1000;      // Volts
    uint32_t voltage_decimal = voltage_mv % 1000;    // Millivolts
    
    // Format message: "Sample #, ADC: XXXX, Voltage: X.XXX V"
    // Using sprintf for formatting, then send via UART
    int len = snprintf(uart_buffer, sizeof(uart_buffer),
                       "Smp %05lu | ADC: %4u | V: %lu.%03lu V\r\n",
                       sample_count, raw_value, voltage_whole, voltage_decimal);
    
    // Send formatted string via UART
    if (len > 0) {
        uart_send_string(uart_buffer);
    }
}

/**
 * @brief System Clock Configuration
 * 
 * Using HSI (High-Speed Internal) oscillator:
 * - Frequency: 16 MHz (default)
 * - No external crystal required
 * - Sufficient for this application
 */
// Clock configuration is done by STM32 bootloader
// Our code uses default 16 MHz HSI

/**
 * @brief Fault Handler - Called on hard fault
 * Indicates system error (useful for debugging)
 */
void HardFault_Handler(void) {
    // Flash LED rapidly to indicate error
    while (1) {
        GPIOC->ODR ^= (1 << 13);
        for (volatile int i = 0; i < 100000; i++);
    }
}