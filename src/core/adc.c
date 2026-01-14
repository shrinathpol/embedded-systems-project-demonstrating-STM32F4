#include "adc.h"

/* ============================================
   Static Variables
   ============================================ */
static volatile uint16_t adc_raw_value = 0;
static volatile adc_status_t adc_status = ADC_STATUS_NOT_READY;
static volatile bool adc_conversion_complete = false;

/* ============================================
   Public Functions
   ============================================ */

adc_status_t adc_init(void) {
    // Enable ADC1 Clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Configure for external triggering
    ADC1->CR2 |= (1U << 28);            // EXTEN = 01 (rising edge)
    ADC1->CR2 |= (6U << 24);            // EXTSEL = 0110 (TIM2 TRGO)
    ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_DDS;

    // Sequence Length = 1
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;

    // Power On
    ADC1->CR2 |= ADC_CR2_ADON;

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

/* ============================================
   ISR - DMA Transfer Complete Handler
   ============================================ */
void DMA2_Stream0_IRQHandler(void) {
    if (DMA2->LISR & DMA_LISR_TCIF0) {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
        adc_conversion_complete = true;
    }
}
