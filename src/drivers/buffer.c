#include "buffer.h"

/* ============================================
   Ring Buffer Implementation
   ============================================ */

bool ring_buffer_init(ring_buffer_t *rb, uint16_t *buffer, uint16_t size) {
    if (rb == NULL || buffer == NULL || size == 0) {
        return false;
    }

    rb->buffer = buffer;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->full = false;

    return true;
}

bool ring_buffer_write(ring_buffer_t *rb, uint16_t data) {
    if (rb == NULL) {
        return false;
    }

    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) % rb->size;
    rb->count++;

    if (rb->head == rb->tail) {
        rb->full = true;
        rb->tail = (rb->tail + 1) % rb->size;
        rb->count = rb->size;
    }

    return true;
}

bool ring_buffer_read(ring_buffer_t *rb, uint16_t *data) {
    if (rb == NULL || data == NULL) {
        return false;
    }

    if (ring_buffer_is_empty(rb)) {
        return false;
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    rb->count--;
    rb->full = false;

    return true;
}

bool ring_buffer_peek(ring_buffer_t *rb, uint16_t offset, uint16_t *data) {
    if (rb == NULL || data == NULL) {
        return false;
    }

    if (offset >= rb->count) {
        return false;
    }

    uint16_t index = (rb->tail + offset) % rb->size;
    *data = rb->buffer[index];

    return true;
}

bool ring_buffer_is_empty(ring_buffer_t *rb) {
    if (rb == NULL) {
        return true;
    }
    return rb->count == 0;
}

bool ring_buffer_is_full(ring_buffer_t *rb) {
    if (rb == NULL) {
        return false;
    }
    return rb->full;
}

uint16_t ring_buffer_count(ring_buffer_t *rb) {
    if (rb == NULL) {
        return 0;
    }
    return rb->count;
}

void ring_buffer_clear(ring_buffer_t *rb) {
    if (rb == NULL) {
        return;
    }

    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->full = false;
}
