#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

/* ============================================
   Ring Buffer (Circular Buffer) Structure
   ============================================ */
typedef struct {
    uint16_t *buffer;                   // Data buffer
    uint16_t head;                      // Write pointer
    uint16_t tail;                      // Read pointer
    uint16_t size;                      // Buffer size
    uint16_t count;                     // Number of elements
    bool full;                          // Buffer full flag
} ring_buffer_t;

/* ============================================
   Public Function Declarations
   ============================================ */

/**
 * @brief Initialize ring buffer
 * @param rb Pointer to ring_buffer_t
 * @param buffer Pointer to data buffer
 * @param size Buffer size in elements
 * @return true if successful
 */
bool ring_buffer_init(ring_buffer_t *rb, uint16_t *buffer, uint16_t size);

/**
 * @brief Add element to buffer (write)
 * @param rb Pointer to ring_buffer_t
 * @param data Data to write
 * @return true if successful
 */
bool ring_buffer_write(ring_buffer_t *rb, uint16_t data);

/**
 * @brief Read element from buffer
 * @param rb Pointer to ring_buffer_t
 * @param data Pointer to data location
 * @return true if successful
 */
bool ring_buffer_read(ring_buffer_t *rb, uint16_t *data);

/**
 * @brief Peek at buffer without removing
 * @param rb Pointer to ring_buffer_t
 * @param offset Offset from head
 * @param data Pointer to data location
 * @return true if successful
 */
bool ring_buffer_peek(ring_buffer_t *rb, uint16_t offset, uint16_t *data);

/**
 * @brief Check if buffer is empty
 * @param rb Pointer to ring_buffer_t
 * @return true if empty
 */
bool ring_buffer_is_empty(ring_buffer_t *rb);

/**
 * @brief Check if buffer is full
 * @param rb Pointer to ring_buffer_t
 * @return true if full
 */
bool ring_buffer_is_full(ring_buffer_t *rb);

/**
 * @brief Get number of elements in buffer
 * @param rb Pointer to ring_buffer_t
 * @return Element count
 */
uint16_t ring_buffer_count(ring_buffer_t *rb);

/**
 * @brief Clear/Reset buffer
 * @param rb Pointer to ring_buffer_t
 */
void ring_buffer_clear(ring_buffer_t *rb);

#endif // __BUFFER_H__
