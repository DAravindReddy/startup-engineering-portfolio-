#include "packet_protocol.h"
#include <string.h>

uint16_t calculate_crc16(const uint8_t *data, uint16_t len) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021; /* CCITT-FALSE polynomial */
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

int16_t encode_packet(uint8_t msg_type, const uint8_t *payload, uint8_t payload_len, uint8_t *out_buffer, uint16_t max_buffer_len) {
    if (!out_buffer || (payload_len > PROTOCOL_MAX_PAYLOAD)) {
        return -1;
    }
    uint16_t required_len = 1 + 1 + 1 + payload_len + 2 + 1; /* Start + Type + Len + Payload + CRC + End */
    if (max_buffer_len < required_len) {
        return -2;
    }

    uint16_t idx = 0;
    out_buffer[idx++] = PROTOCOL_START_BYTE;
    out_buffer[idx++] = msg_type;
    out_buffer[idx++] = payload_len;
    if (payload_len > 0 && payload) {
        memcpy(&out_buffer[idx], payload, payload_len);
        idx += payload_len;
    }

    /* CRC computed over Type + Len + Payload */
    uint16_t crc = calculate_crc16(&out_buffer[1], 2 + payload_len);
    out_buffer[idx++] = (uint8_t)((crc >> 8) & 0xFF);
    out_buffer[idx++] = (uint8_t)(crc & 0xFF);
    out_buffer[idx++] = PROTOCOL_END_BYTE;

    return (int16_t)idx;
}

bool decode_packet(const uint8_t *raw_stream, uint16_t stream_len, telemetry_packet_t *out_packet) {
    if (!raw_stream || !out_packet || stream_len < 6) {
        return false;
    }

    /* Scan for Start Byte */
    for (uint16_t i = 0; i < stream_len - 5; i++) {
        if (raw_stream[i] == PROTOCOL_START_BYTE) {
            uint8_t msg_type = raw_stream[i + 1];
            uint8_t payload_len = raw_stream[i + 2];
            if (payload_len > PROTOCOL_MAX_PAYLOAD) continue;

            uint16_t expected_total = 1 + 1 + 1 + payload_len + 2 + 1;
            if (i + expected_total > stream_len) continue;

            if (raw_stream[i + expected_total - 1] != PROTOCOL_END_BYTE) continue;

            uint16_t received_crc = ((uint16_t)raw_stream[i + 3 + payload_len] << 8) |
                                     (uint16_t)raw_stream[i + 4 + payload_len];

            uint16_t computed_crc = calculate_crc16(&raw_stream[i + 1], 2 + payload_len);
            if (received_crc == computed_crc) {
                out_packet->start_byte = PROTOCOL_START_BYTE;
                out_packet->msg_type = msg_type;
                out_packet->payload_len = payload_len;
                if (payload_len > 0) {
                    memcpy(out_packet->payload, &raw_stream[i + 3], payload_len);
                }
                out_packet->crc16 = received_crc;
                out_packet->end_byte = PROTOCOL_END_BYTE;
                return true;
            }
        }
    }
    return false;
}
