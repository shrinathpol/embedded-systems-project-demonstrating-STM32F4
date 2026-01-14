#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "../config.h"

/* ============================================
   ADC Status Enumeration
   ============================================ */
typedef enum {
    ADC_STATUS_OK = 0,
    ADC_STATUS_ERROR = 1,
    ADC_STATUS_NOT_READY = 2,
    ADC_STATUS_TIMEOUT = 3
} adc_status_t;

/* ============================================
   ADC Conversion Data Structure
   ============================================ */
typedef struct {
    uint16_t raw_value;                 // Raw ADC count (0-4095)
    uint32_t voltage_mv;                // Voltage in millivolts
    uint32_t voltage_whole;             // Whole volts (V)
    uint32_t voltage_decimal;           // Decimal portion (mV)
    uint64_t timestamp_ms;              // Timestamp
    adc_status_t status;                // Conversion status
} adc_reading_t;

/* ============================================
   Public Function Declarations
   ============================================ */

/**
 * @brief Initialize ADC1 with DMA trigger
 * @return ADC status
 */
adc_status_t adc_init(void);

/**
 * @brief Start ADC conversions
 */
void adc_start(void);

/**
 * @brief Stop ADC conversions
 */
void adc_stop(void);

/**
 * @brief Get latest ADC reading
 * @param reading Pointer to adc_reading_t structure
 * @return ADC status
 */
adc_status_t adc_get_reading(adc_reading_t *reading);

/**
 * @brief Convert raw ADC value to voltage
 * @param raw_value Raw ADC count
 * @return Voltage in millivolts
 */
uint32_t adc_raw_to_voltage_mv(uint16_t raw_value);

/**
 * @brief Check if ADC is ready
 * @return true if ready, false otherwise
 */
bool adc_is_ready(void);

/**
 * @brief Get ADC status
 * @return Current ADC status
 */
adc_status_t adc_get_status(void);

#endif // __ADC_H__
