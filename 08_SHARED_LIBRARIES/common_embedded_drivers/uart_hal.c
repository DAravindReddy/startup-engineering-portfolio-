#include "uart_hal.h"
#include <string.h>

void uart_ring_buffer_init(uart_ring_buffer_t *rb) {
    if (!rb) return;
    rb->head = 0;
    rb->tail = 0;
    memset(rb->buffer, 0, sizeof(rb->buffer));
}

bool uart_ring_buffer_put(uart_ring_buffer_t *rb, uint8_t byte) {
    if (!rb) return false;
    uint16_t next = (rb->head + 1) % UART_RING_BUFFER_SIZE;
    if (next == rb->tail) {
        return false; /* Buffer overflow */
    }
    rb->buffer[rb->head] = byte;
    rb->head = next;
    return true;
}

bool uart_ring_buffer_get(uart_ring_buffer_t *rb, uint8_t *byte) {
    if (!rb || !byte) return false;
    if (rb->head == rb->tail) {
        return false; /* Buffer empty */
    }
    *byte = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % UART_RING_BUFFER_SIZE;
    return true;
}

uint16_t uart_ring_buffer_available(const uart_ring_buffer_t *rb) {
    if (!rb) return 0;
    if (rb->head >= rb->tail) {
        return rb->head - rb->tail;
    }
    return UART_RING_BUFFER_SIZE + rb->head - rb->tail;
}
