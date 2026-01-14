#include "../include/core/adc.h"

/* ============================================
   Static Variables
   ============================================ */
static volatile uint16_t adc_raw_value = 0;
static volatile adc_status_t adc_status = ADC_STATUS_NOT_READY;
static volatile bool adc_conversion_complete = false;

/* ============================================
   ADC Initialization
   ============================================ */

/**
 * @brief Initialize ADC1 for timer-triggered sampling
 * 
 * Configuration:
 * - ADC1 Channel 0 (PA0)
 * - External trigger: TIM2 TRGO (trigger output)
 * - Trigger edge: Rising edge
 * - DMA enabled for data transfer
 * - Single-channel conversion (Channel 0 only)
 * - 12-bit resolution (default)
 * - Sample time: 15 cycles (default)
 * 
 * Trigger mapping:
 * - EXTSEL = 0110 (TIM2 TRGO)
 * - EXTEN = 01 (rising edge trigger)
 * 
 * @return ADC status
 */
adc_status_t adc_init(void) {
    // Enable ADC1 clock (APB2)
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    
    // Configure ADC control register 1
    ADC1->CR1 = 0;
    ADC1->CR1 &= ~ADC_CR1_SCAN;     // Single channel mode
    
    // Configure ADC control register 2
    // Clear settings first
    ADC1->CR2 = 0;
    
    // Enable DMA mode
    ADC1->CR2 |= ADC_CR2_DMA;
    
    // Enable DMA requests to be issued as each conversion finishes
    ADC1->CR2 |= ADC_CR2_DDS;
    
    // Configure external trigger
    // EXTEN[1:0] = 01 (rising edge trigger)
    ADC1->CR2 |= (1U << 28);
    
    // EXTSEL[3:0] = 0110 (TIM2 TRGO)
    ADC1->CR2 |= (6U << 24);
    
    // Sequence Length = 1 (single channel)
    ADC1->SQR1 &= ~ADC_SQR1_L;      // L[3:0] = 0000 (length = 1)
    
    // Select Channel 0 (PA0) for conversion
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;    // Clear SQ1[4:0]
    ADC1->SQR3 |= (0U << 0);        // SQ1 = 0 (Channel 0)
    
    // Enable ADC
    ADC1->CR2 |= ADC_CR2_ADON;
    
    // Small delay for ADC to power up
    for (volatile int i = 0; i < 100; i++);
    
    adc_status = ADC_STATUS_OK;
    return adc_status;
}

void adc_start(void) {
    ADC1->CR2 |= ADC_CR2_ADON;
    adc_conversion_complete = false;
}

void adc_stop(void) {
    ADC1->CR2 &= ~ADC_CR2_ADON;
}

adc_status_t adc_get_reading(adc_reading_t *reading) {
    if (reading == NULL) {
        return ADC_STATUS_ERROR;
    }

    if (!adc_conversion_complete) {
        return ADC_STATUS_NOT_READY;
    }

    reading->raw_value = adc_raw_value;
    reading->voltage_mv = adc_raw_to_voltage_mv(adc_raw_value);
    reading->voltage_whole = reading->voltage_mv / 1000;
    reading->voltage_decimal = reading->voltage_mv % 1000;
    reading->status = adc_status;

    adc_conversion_complete = false;
    return ADC_STATUS_OK;
}

uint32_t adc_raw_to_voltage_mv(uint16_t raw_value) {
    // Voltage(mV) = (ADC_Value × Reference) / Max_Count
    // Using integer math: (ADC * 3300) / 4095
    return (uint32_t)((raw_value * ADC_REFERENCE_MV) / ADC_MAX_VALUE);
}

bool adc_is_ready(void) {
    return adc_status == ADC_STATUS_OK;
}

adc_status_t adc_get_status(void) {
    return adc_status;
}
