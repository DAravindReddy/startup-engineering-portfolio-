#ifndef PACKET_PROTOCOL_H
#define PACKET_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PROTOCOL_START_BYTE 0xAA
#define PROTOCOL_END_BYTE   0x55
#define PROTOCOL_MAX_PAYLOAD 64

#pragma pack(push, 1)
typedef struct {
    uint8_t start_byte;
    uint8_t msg_type;
    uint8_t payload_len;
    uint8_t payload[PROTOCOL_MAX_PAYLOAD];
    uint16_t crc16;
    uint8_t end_byte;
} telemetry_packet_t;
#pragma pack(pop)

uint16_t calculate_crc16(const uint8_t *data, uint16_t len);
int16_t encode_packet(uint8_t msg_type, const uint8_t *payload, uint8_t payload_len, uint8_t *out_buffer, uint16_t max_buffer_len);
bool decode_packet(const uint8_t *raw_stream, uint16_t stream_len, telemetry_packet_t *out_packet);

#ifdef __cplusplus
}
#endif

#endif /* PACKET_PROTOCOL_H */
