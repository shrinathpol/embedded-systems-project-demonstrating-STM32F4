#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdint.h>
#include <stdbool.h>

/* ============================================
   Error Codes
   ============================================ */
typedef enum {
    ERROR_NONE = 0,
    ERROR_ADC_FAILED = 0x01,
    ERROR_DMA_FAILED = 0x02,
    ERROR_UART_FAILED = 0x04,
    ERROR_TIMER_FAILED = 0x08,
    ERROR_BUFFER_OVERFLOW = 0x10,
    ERROR_BUFFER_UNDERFLOW = 0x20,
    ERROR_INVALID_PARAM = 0x40,
    ERROR_TIMEOUT = 0x80,
    ERROR_UNKNOWN = 0xFF
} error_code_t;

/* ============================================
   Error Status Structure
   ============================================ */
typedef struct {
    error_code_t code;                  // Error code
    uint32_t timestamp_ms;              // When error occurred
    uint8_t severity;                   // 0=Info, 1=Warning, 2=Error, 3=Critical
    const char *message;                // Error message
} error_t;

/* ============================================
   Public Function Declarations
   ============================================ */

/**
 * @brief Initialize error handling
 */
void error_init(void);

/**
 * @brief Report error
 * @param code Error code
 * @param severity Error severity (0-3)
 * @param message Error message
 */
void error_report(error_code_t code, uint8_t severity, const char *message);

/**
 * @brief Get last error
 * @return Last error structure
 */
error_t error_get_last(void);

/**
 * @brief Clear error history
 */
void error_clear(void);

/**
 * @brief Check if critical error occurred
 * @return true if critical error present
 */
bool error_is_critical(void);

/**
 * @brief Get error count
 * @return Number of errors recorded
 */
uint16_t error_get_count(void);

/**
 * @brief Get error message string
 * @param code Error code
 * @return Error message string
 */
const char* error_get_string(error_code_t code);

#endif // __ERROR_H__
