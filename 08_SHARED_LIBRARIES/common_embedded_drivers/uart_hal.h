#ifndef UART_HAL_H
#define UART_HAL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UART_RING_BUFFER_SIZE 128

typedef struct {
    uint8_t buffer[UART_RING_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} uart_ring_buffer_t;

typedef enum {
    UART_OK = 0,
    UART_ERR_OVERFLOW = -1,
    UART_ERR_TIMEOUT = -2,
    UART_ERR_PARAM = -3
} uart_status_t;

void uart_ring_buffer_init(uart_ring_buffer_t *rb);
bool uart_ring_buffer_put(uart_ring_buffer_t *rb, uint8_t byte);
bool uart_ring_buffer_get(uart_ring_buffer_t *rb, uint8_t *byte);
uint16_t uart_ring_buffer_available(const uart_ring_buffer_t *rb);

uart_status_t uart_init(uint32_t baud_rate);
uart_status_t uart_send_byte(uint8_t byte);
uart_status_t uart_send_string(const char *str);
uart_status_t uart_send_buffer(const uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* UART_HAL_H */
