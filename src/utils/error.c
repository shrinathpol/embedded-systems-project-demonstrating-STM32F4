#include "utils/error.h"
#include <string.h>

/* ============================================
   Static Variables
   ============================================ */
#define MAX_ERROR_HISTORY 10

static error_t error_history[MAX_ERROR_HISTORY] = {0};
static uint16_t error_count = 0;
static uint16_t error_index = 0;
static bool critical_error_flag = false;

/* ============================================
   Public Functions
   ============================================ */

void error_init(void) {
    error_count = 0;
    error_index = 0;
    critical_error_flag = false;
    memset(error_history, 0, sizeof(error_history));
}

void error_report(error_code_t code, uint8_t severity, const char *message) {
    error_t *err = &error_history[error_index];

    err->code = code;
    err->severity = severity;
    err->message = message;
    // err->timestamp_ms = get_system_time_ms(); // TODO: Implement

    error_count++;
    if (severity >= 3) {  // Critical
        critical_error_flag = true;
    }

    error_index = (error_index + 1) % MAX_ERROR_HISTORY;
}

error_t error_get_last(void) {
    uint16_t last_index = (error_index > 0) ? (error_index - 1) : (MAX_ERROR_HISTORY - 1);
    return error_history[last_index];
}

void error_clear(void) {
    error_count = 0;
    error_index = 0;
    critical_error_flag = false;
    memset(error_history, 0, sizeof(error_history));
}

bool error_is_critical(void) {
    return critical_error_flag;
}

uint16_t error_get_count(void) {
    return error_count;
}

const char* error_get_string(error_code_t code) {
    switch (code) {
        case ERROR_NONE:
            return "No error";
        case ERROR_ADC_FAILED:
            return "ADC conversion failed";
        case ERROR_DMA_FAILED:
            return "DMA transfer failed";
        case ERROR_UART_FAILED:
            return "UART communication failed";
        case ERROR_TIMER_FAILED:
            return "Timer initialization failed";
        case ERROR_BUFFER_OVERFLOW:
            return "Buffer overflow";
        case ERROR_BUFFER_UNDERFLOW:
            return "Buffer underflow";
        case ERROR_INVALID_PARAM:
            return "Invalid parameter";
        case ERROR_TIMEOUT:
            return "Operation timeout";
        default:
            return "Unknown error";
    }
}
