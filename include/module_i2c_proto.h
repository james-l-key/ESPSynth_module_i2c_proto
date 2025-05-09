#ifndef MODULE_I2C_PROTO_H
#define MODULE_I2C_PROTO_H

#include <stdint.h>
#include <stddef.h>  // For size_t
#include <stdbool.h> // For bool type
#include "esp_err.h"

/**
 * @file module_i2c_proto.h
 * @brief I2C communication protocol for ESPSynth modules
 *
 * This header defines the protocol used for communication between
 * the Central Controller and various ESPSynth modules.
 *
 * @note This is a shared component used by all ESPSynth modules and the Central Controller
 */

/**
 * @defgroup common_registers Common Registers
 * @brief Registers common to all module types
 * @{
 */
#define REG_COMMON_MODULE_TYPE        0x00 /**< Module type identifier */
#define REG_COMMON_FIRMWARE_VERSION   0x01 /**< Firmware version */
#define REG_COMMON_STATUS             0x02 /**< Module status */
#define REG_COMMON_I2S_CONFIG         0x03 /**< I2S configuration */
#define REG_COMMON_SET_PARAM          0x04 /**< Set parameter */
#define REG_COMMON_GET_PARAM          0x05 /**< Get parameter */
/** @} */

/**
 * @defgroup common_commands Common Commands
 * @brief Commands common to all module types
 * @{
 */
#define CMD_COMMON_RESET              0xF0 /**< Reset module */
#define CMD_COMMON_SAVE_SETTINGS      0xF1 /**< Save settings to NVS */
#define CMD_COMMON_LOAD_SETTINGS      0xF2 /**< Load settings from NVS */
/** @} */

/**
 * @defgroup module_types Module Types
 * @brief Identifiers for different module types
 * @{
 */
#define MODULE_TYPE_CENTRAL           0x00 /**< Central Controller */
#define MODULE_TYPE_OSCILLATOR        0x01 /**< Basic Oscillator */
#define MODULE_TYPE_FILTER            0x02 /**< Filter */
#define MODULE_TYPE_ENVELOPE          0x03 /**< Envelope Generator */
#define MODULE_TYPE_LFO               0x04 /**< Low Frequency Oscillator */
#define MODULE_TYPE_MIXER             0x05 /**< Audio Mixer */
#define MODULE_TYPE_EFFECTS           0x06 /**< Effects Processor */
/** @} */

/**
 * @defgroup status_flags Status Flags
 * @brief Bit flags for the status register
 * @{
 */
#define STATUS_INITIALIZED            (1 << 0) /**< Module is initialized */
#define STATUS_ERROR                  (1 << 1) /**< Module has an error */
#define STATUS_BUSY                   (1 << 2) /**< Module is busy processing */
#define STATUS_AUDIO_ACTIVE           (1 << 3) /**< Module is generating/processing audio */
#define STATUS_PARAM_CHANGED          (1 << 4) /**< Parameter has changed */
/** @} */

/**
 * @defgroup osc_params Oscillator Parameters
 * @brief Parameter IDs for oscillator modules
 * @{
 */
#define PARAM_OSC_WAVEFORM            0x10 /**< Waveform type */
#define PARAM_OSC_PITCH_MIDI          0x11 /**< Pitch as MIDI note number */
#define PARAM_OSC_PITCH_FIXED_HZ      0x12 /**< Pitch as frequency in Hz */
#define PARAM_OSC_LEVEL_U16           0x13 /**< Level (0-65535) */
#define PARAM_OSC_PW_U16              0x14 /**< Pulse width (0-65535) */
#define PARAM_OSC_DETUNE_S16          0x15 /**< Detune in cents (-8192 to 8191) */
/** @} */

/**
 * @defgroup filter_params Filter Parameters
 * @brief Parameter IDs for filter modules
 * @{
 */
#define PARAM_FILTER_TYPE             0x20 /**< Filter type */
#define PARAM_FILTER_CUTOFF_U16       0x21 /**< Cutoff frequency (0-65535) */
#define PARAM_FILTER_RESONANCE_U16    0x22 /**< Resonance/Q (0-65535) */
#define PARAM_FILTER_GAIN_S16         0x23 /**< Gain in dB (-8192 to 8191) */
/** @} */

/**
 * @brief Initialize the I2C protocol handler
 * 
 * @param module_type The module type identifier
 * @param default_address Default I2C address to use if not found in NVS
 * @return ESP_OK if initialization is successful, error code otherwise
 */
esp_err_t module_i2c_proto_init(uint8_t module_type, uint8_t default_address);

/**
 * @brief Process an incoming I2C command
 * 
 * @param cmd_buffer Pointer to the command buffer
 * @param cmd_len Length of the command data
 * @param[out] resp_buffer Pointer to the response buffer
 * @param[in,out] resp_len Maximum length of response buffer on input, actual response length on output
 * @return ESP_OK if command processed successfully, error code otherwise
 */
esp_err_t module_i2c_proto_process_command(const uint8_t *cmd_buffer, size_t cmd_len, 
                                          uint8_t *resp_buffer, size_t *resp_len);

/**
 * @brief Set a parameter value
 * 
 * @param param_id The parameter identifier
 * @param value Pointer to the parameter value data
 * @param value_len Length of the parameter value data
 * @return ESP_OK if parameter set successfully, error code otherwise
 */
esp_err_t module_i2c_proto_set_param(uint8_t param_id, const void *value, size_t value_len);

/**
 * @brief Get a parameter value
 * 
 * @param param_id The parameter identifier
 * @param[out] value Pointer to store the parameter value
 * @param[in,out] value_len Maximum length of value buffer on input, actual parameter length on output
 * @return ESP_OK if parameter retrieved successfully, error code otherwise
 */
esp_err_t module_i2c_proto_get_param(uint8_t param_id, void *value, size_t *value_len);

/**
 * @brief Register parameter change callback
 * 
 * @param param_id Parameter ID to monitor
 * @param callback Function to call when parameter changes
 * @param user_data User data to pass to the callback
 * @return ESP_OK if callback registered successfully, error code otherwise
 */
esp_err_t module_i2c_proto_register_param_callback(uint8_t param_id, 
                                                 void (*callback)(void *user_data, const void *value, size_t value_len), 
                                                 void *user_data);

#endif /* MODULE_I2C_PROTO_H */