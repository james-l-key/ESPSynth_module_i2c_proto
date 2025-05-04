#include "module_i2c_proto.h"
#include <string.h> // For memcpy
#include <stdint.h>

// Implementation for i2c_proto_pack_set_param_msg
size_t i2c_proto_pack_set_param_msg(uint8_t *buf, size_t buf_len, ParamId_t param_id, ParamValue_t param_value)
{
    const size_t required_len = 1 + sizeof(SetParamPayload_t); // Command + Payload
    if (!buf || buf_len < required_len)
    {
        return 0; // Error: Null buffer or buffer too small
    }

    SetParamPayload_t payload = {
        .param_id = param_id, // Assuming direct assignment works (check endianness if MCUs differ, unlikely here)
        .param_value = param_value
    };

    buf[0] = REG_COMMON_SET_PARAM; // The command byte
    memcpy(buf + 1, &payload, sizeof(SetParamPayload_t));

    return required_len;
}

// Implementation for i2c_proto_unpack_set_param_payload
bool i2c_proto_unpack_set_param_payload(const uint8_t *payload_buf, size_t payload_len, ParamId_t *param_id, ParamValue_t *param_value)
{
    if (!payload_buf || !param_id || !param_value || payload_len != sizeof(SetParamPayload_t))
    {
        return false; // Error: Invalid args or length
    }

    SetParamPayload_t payload;
    memcpy(&payload, payload_buf, sizeof(SetParamPayload_t));

    *param_id = payload.param_id; // Assuming direct assignment works
    *param_value = payload.param_value;

    return true;
}

// Implementation for i2c_proto_pack_i2s_config_msg
size_t i2c_proto_pack_i2s_config_msg(uint8_t *buf, size_t buf_len, const I2sConfig_t *config)
{
    const size_t required_len = 1 + sizeof(I2sConfig_t); // Command + Payload
    if (!buf || !config || buf_len < required_len)
    {
        return 0; // Error: Null buffer or buffer too small
    }

    buf[0] = REG_COMMON_I2S_CONFIG; // The command byte
    memcpy(buf + 1, config, sizeof(I2sConfig_t));

    return required_len;
}

// Implementation for i2c_proto_unpack_i2s_config_payload
bool i2c_proto_unpack_i2s_config_payload(const uint8_t *payload_buf, size_t payload_len, I2sConfig_t *config)
{
    if (!payload_buf || !config || payload_len != sizeof(I2sConfig_t))
    {
        return false; // Error: Invalid args or length
    }
    memcpy(config, payload_buf, sizeof(I2sConfig_t));
    return true;
}

// Add implementations for other pack/unpack helpers if defined