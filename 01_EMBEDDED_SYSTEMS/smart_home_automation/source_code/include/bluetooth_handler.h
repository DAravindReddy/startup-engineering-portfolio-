#ifndef BLUETOOTH_HANDLER_H
#define BLUETOOTH_HANDLER_H

#include <stdint.h>
#include <stdbool.h>

#define BT_CMD_BUFFER_MAX 32

typedef enum {
    CMD_NONE = 0,
    CMD_LED1_ON,
    CMD_LED1_OFF,
    CMD_LED2_ON,
    CMD_LED2_OFF,
    CMD_MOTOR_ON,
    CMD_MOTOR_OFF,
    CMD_MOTOR_SPEED,    /* Payload: integer speed 0 - 255 */
    CMD_SERVO_ANGLE,    /* Payload: integer angle 0 - 180 */
    CMD_PUMP_ON,
    CMD_PUMP_OFF,
    CMD_MODE_AUTO,
    CMD_MODE_MANUAL,
    CMD_SET_THRESHOLD,  /* Payload: integer threshold % */
    CMD_GET_STATUS,
    CMD_UNKNOWN
} bt_command_type_t;

typedef struct {
    bt_command_type_t type;
    int16_t param_val;
} bt_parsed_command_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parses an incoming raw null-terminated command string into a structured command.
 */
bt_parsed_command_t parse_bluetooth_command(const char *cmd_str);

#ifdef __cplusplus
}
#endif

#endif /* BLUETOOTH_HANDLER_H */
