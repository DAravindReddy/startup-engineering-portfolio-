#include "bluetooth_handler.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static void trim_whitespace(char *str) {
    if (!str) return;
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == '\r' || str[len - 1] == '\n' || isspace((unsigned char)str[len - 1]))) {
        str[--len] = '\0';
    }
}

bt_parsed_command_t parse_bluetooth_command(const char *cmd_str) {
    bt_parsed_command_t result;
    result.type = CMD_NONE;
    result.param_val = 0;

    if (!cmd_str || cmd_str[0] == '\0') {
        return result;
    }

    char clean[BT_CMD_BUFFER_MAX];
    strncpy(clean, cmd_str, BT_CMD_BUFFER_MAX - 1);
    clean[BT_CMD_BUFFER_MAX - 1] = '\0';
    trim_whitespace(clean);

    if (strcmp(clean, "L1_ON") == 0) {
        result.type = CMD_LED1_ON;
    } else if (strcmp(clean, "L1_OFF") == 0) {
        result.type = CMD_LED1_OFF;
    } else if (strcmp(clean, "L2_ON") == 0) {
        result.type = CMD_LED2_ON;
    } else if (strcmp(clean, "L2_OFF") == 0) {
        result.type = CMD_LED2_OFF;
    } else if (strcmp(clean, "M_ON") == 0) {
        result.type = CMD_MOTOR_ON;
    } else if (strcmp(clean, "M_OFF") == 0) {
        result.type = CMD_MOTOR_OFF;
    } else if (strncmp(clean, "M_SPEED:", 8) == 0) {
        result.type = CMD_MOTOR_SPEED;
        result.param_val = (int16_t)atoi(&clean[8]);
        if (result.param_val < 0) result.param_val = 0;
        if (result.param_val > 255) result.param_val = 255;
    } else if (strncmp(clean, "SERVO:", 6) == 0) {
        result.type = CMD_SERVO_ANGLE;
        result.param_val = (int16_t)atoi(&clean[6]);
        if (result.param_val < 0) result.param_val = 0;
        if (result.param_val > 180) result.param_val = 180;
    } else if (strcmp(clean, "PUMP_ON") == 0) {
        result.type = CMD_PUMP_ON;
    } else if (strcmp(clean, "PUMP_OFF") == 0) {
        result.type = CMD_PUMP_OFF;
    } else if (strcmp(clean, "MODE_AUTO") == 0) {
        result.type = CMD_MODE_AUTO;
    } else if (strcmp(clean, "MODE_MANUAL") == 0) {
        result.type = CMD_MODE_MANUAL;
    } else if (strncmp(clean, "SET_TH:", 7) == 0) {
        result.type = CMD_SET_THRESHOLD;
        result.param_val = (int16_t)atoi(&clean[7]);
        if (result.param_val < 5) result.param_val = 5;
        if (result.param_val > 95) result.param_val = 95;
    } else if (strcmp(clean, "STATUS?") == 0 || strcmp(clean, "GET_SENSORS") == 0) {
        result.type = CMD_GET_STATUS;
    } else {
        result.type = CMD_UNKNOWN;
    }

    return result;
}
