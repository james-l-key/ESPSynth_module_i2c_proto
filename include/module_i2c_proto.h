#pragma once

#include <stdint.h>
#include <stddef.h>  // For size_t
#include <stdbool.h> // For bool type

#ifdef __cplusplus
extern "C"
{
#endif

    //------------------------------------------------------------------------------
    // General Module Identification / Status Registers (Read Only by Master)
    //------------------------------------------------------------------------------
    typedef enum
    {
        // Common Registers (Readable by Master)
        REG_COMMON_MODULE_TYPE = 0x00,      // R: Returns ModuleType_t enum value (1 byte)
        REG_COMMON_FIRMWARE_VERSION = 0x01, // R: Returns uint16_t version (e.g., 0x0100 for 1.0.0) (2 bytes)
        REG_COMMON_STATUS = 0x02,           // R: Returns uint8_t status bits (e.g., error flags) (1 byte)
        REG_COMMON_UID_PART1 = 0x03,        // R: Optional: Part 1 of unique ID (e.g., from MAC) (4 bytes)
        REG_COMMON_UID_PART2 = 0x04,        // R: Optional: Part 2 of unique ID (e.g., from MAC) (2 bytes)

        // --- Add more common read registers if needed ---

        // Start of module-specific readable registers (Example range)
        REG_SPECIFIC_READ_START = 0x20,

    } CommonReadRegAddr_t;

    //------------------------------------------------------------------------------
    // General Module Configuration / Command Registers (Write Only or Read/Write)
    //------------------------------------------------------------------------------
    typedef enum
    {
        // Common Writable Registers/Commands (Writeable by Master)
        CMD_COMMON_RESET = 0x80,         // W: Trigger soft reset (0 bytes payload)
        REG_COMMON_I2S_CONFIG = 0x81,    // W: Configure I2S TDM slots (Payload: I2sConfig_t structure)
        REG_COMMON_SET_PARAM = 0x82,     // W: Set a specific parameter (Payload: SetParamPayload_t)
        CMD_COMMON_SAVE_SETTINGS = 0x83, // W: Request module to save configurable settings to its NVS (0 bytes payload)

        // --- Add more common write registers/commands if needed ---

        // Start of module-specific writeable registers (Example range)
        REG_SPECIFIC_WRITE_START = 0xA0,

    } CommonWriteRegAddr_t;

    //------------------------------------------------------------------------------
    // Module Type Definitions
    //------------------------------------------------------------------------------
    typedef enum
    {
        MODULE_TYPE_UNKNOWN = 0x00,
        MODULE_TYPE_CENTRAL_CTRL = 0x01, // Should not respond as slave normally
        MODULE_TYPE_OSCILLATOR = 0x10,
        MODULE_TYPE_FILTER = 0x11,
        MODULE_TYPE_EFFECT_REVERB = 0x20,
        MODULE_TYPE_EFFECT_DELAY = 0x21,
        MODULE_TYPE_LFO = 0x30,
        MODULE_TYPE_MIXER = 0x40,
        MODULE_TYPE_ADC_INPUT = 0x50,
        MODULE_TYPE_DAC_OUTPUT = 0x51, // Host of MCLK, may not need extensive slave config
        MODULE_TYPE_ROUTING_IN = 0x60,
        MODULE_TYPE_ROUTING_OUT = 0x61,
        // Add more types...
    } ModuleType_t; // Use uint8_t for storage/transmission

//------------------------------------------------------------------------------
// Common Status Bit Definitions (for REG_COMMON_STATUS)
//------------------------------------------------------------------------------
#define STATUS_BIT_ERROR (1 << 0)          // General unspecified error flag
#define STATUS_BIT_I2S_ERROR (1 << 1)      // I2S configuration/sync error reported by module
#define STATUS_BIT_DSP_OVERLOAD (1 << 2)   // DSP processing took too long in the last cycle
#define STATUS_BIT_CONFIG_PENDING (1 << 3) // Module needs configuration (e.g., I2S slots)
#define STATUS_BIT_BUSY (1 << 4)           // Module is busy with an internal operation (e.g., saving to NVS)
    // Add more status bits...

    //------------------------------------------------------------------------------
    // I2S Configuration Structure (Payload for REG_COMMON_I2S_CONFIG)
    // Defines which TDM slots the module should listen to or write to.
    //------------------------------------------------------------------------------
    typedef struct __attribute__((packed))
    {
        uint16_t input_slot_mask;  // Bitmask indicating which TDM slots (0-15) module should read from (if any)
        uint16_t output_slot_mask; // Bitmask indicating which TDM slots (0-15) module should write to (if any)
        // Example: input_slot_mask = 0x0003 means read from slot 0 and 1
        // Example: output_slot_mask = 0x0008 means write to slot 3
    } I2sConfig_t; // Size = 4 bytes

    //------------------------------------------------------------------------------
    // Parameter ID Definitions (Used with REG_COMMON_SET_PARAM)
    // Grouped by module type for clarity using ranges. uint16_t allows ~65k params.
    //------------------------------------------------------------------------------
    typedef uint16_t ParamId_t;

// Define parameter ID ranges
#define PARAM_RANGE_COMMON 0x0000
#define PARAM_RANGE_OSC 0x1000
#define PARAM_RANGE_FILTER 0x1100
#define PARAM_RANGE_LFO 0x3000
    // ... other ranges

    // Parameter IDs (Examples)
    typedef enum
    {
        // Oscillator Parameters (0x1000 - 0x10FF)
        PARAM_OSC_WAVEFORM = PARAM_RANGE_OSC | 0x0001,         // Value: OscWaveform_t enum (uint8_t fits in ParamValue_t.u8[0])
        PARAM_OSC_FREQUENCY_PITCH = PARAM_RANGE_OSC | 0x0002,  // Value: Pitch representation (e.g., MIDI note number uint8_t, or fine pitch uint16_t/s16_t)
        PARAM_OSC_FREQUENCY_FINE = PARAM_RANGE_OSC | 0x0003,   // Value: Fine tune cents (e.g., s16_t)
        PARAM_OSC_LEVEL = PARAM_RANGE_OSC | 0x0004,            // Value: Output level (e.g., uint16_t, 0-65535 maps to 0.0-1.0)
        PARAM_OSC_AMP_MOD_SLOT = PARAM_RANGE_OSC | 0x0005,     // Value: TDM Slot number (0-15) for Amp CV, or 0xFF for none (uint8_t)
        PARAM_OSC_FREQ_MOD_SLOT = PARAM_RANGE_OSC | 0x0006,    // Value: TDM Slot number (0-15) for Freq CV, or 0xFF for none (uint8_t)
        PARAM_OSC_PW = PARAM_RANGE_OSC | 0x0007,               // Value: Pulse Width (uint16_t 0-65535) for applicable waveforms
        PARAM_OSC_SYNC_SOURCE_SLOT = PARAM_RANGE_OSC | 0x0008, // Value: TDM Slot number (0-15) for Sync source, or 0xFF for none (uint8_t)

        // Filter Parameters (0x1100 - 0x11FF)
        PARAM_FILTER_TYPE = PARAM_RANGE_FILTER | 0x0001,            // Value: FilterType_t enum (uint8_t)
        PARAM_FILTER_CUTOFF_PITCH = PARAM_RANGE_FILTER | 0x0002,    // Value: Cutoff pitch representation (e.g., MIDI note uint8_t)
        PARAM_FILTER_CUTOFF_FINE = PARAM_RANGE_FILTER | 0x0003,     // Value: Cutoff fine tune cents (e.g., s16_t)
        PARAM_FILTER_RESONANCE = PARAM_RANGE_FILTER | 0x0004,       // Value: Q factor representation (e.g., uint16_t 0-65535)
        PARAM_FILTER_CUTOFF_MOD_SLOT = PARAM_RANGE_FILTER | 0x0005, // Value: TDM Slot number (0-15) for Cutoff CV, or 0xFF for none (uint8_t)

        // LFO Parameters (0x3000 - 0x30FF)
        PARAM_LFO_WAVEFORM = PARAM_RANGE_LFO | 0x0001,       // Value: LfoWaveform_t enum (uint8_t)
        PARAM_LFO_RATE_HZ = PARAM_RANGE_LFO | 0x0002,        // Value: Rate in Hz (e.g., Q16.16 fixed point uint32_t, or maybe just uint16_t milliHz)
        PARAM_LFO_DEPTH = PARAM_RANGE_LFO | 0x0003,          // Value: Amplitude (e.g., uint16_t 0-65535)
        PARAM_LFO_SYNC_CLOCK_DIV = PARAM_RANGE_LFO | 0x0004, // Value: Clock division if synced (uint16_t)

        // Define enums for other module types (FX, Mixer, ADC, etc.) similarly

    } ParamIdValue_t; // Example enum names, use ParamId_t type

    //------------------------------------------------------------------------------
    // Parameter Value Representation (Used with REG_COMMON_SET_PARAM)
    // Using a fixed size union (4 bytes) for simplicity. The interpretation of the
    // value depends on the ParamId_t used.
    //------------------------------------------------------------------------------
    typedef union
    {
        uint32_t u32;
        int32_t s32;
        uint16_t u16[2]; // Access as value.u16[0], value.u16[1]
        int16_t s16[2];  // Access as value.s16[0], value.s16[1]
        uint8_t u8[4];   // Access as value.u8[0]..u8[3]
        // float    f32; // Avoid if possible due to endianness and overhead. Prefer fixed-point or scaled integers.
    } ParamValue_t; // Size = 4 bytes

    //------------------------------------------------------------------------------
    // Parameter Setting Structure (Payload for REG_COMMON_SET_PARAM)
    //------------------------------------------------------------------------------
    typedef struct __attribute__((packed))
    {
        ParamId_t param_id;       // 2 bytes
        ParamValue_t param_value; // 4 bytes
    } SetParamPayload_t;          // Total size = 6 bytes

    //------------------------------------------------------------------------------
    // Waveform/Type Enums (Example - define values as needed)
    //------------------------------------------------------------------------------
    typedef enum
    {
        OSC_WAVE_SINE,
        OSC_WAVE_TRIANGLE,
        OSC_WAVE_SAW,
        OSC_WAVE_SQUARE,
        OSC_WAVE_PULSE /*...*/
    } OscWaveform_t;   // Fits in uint8_t
    typedef enum
    {
        FILTER_TYPE_LPF,
        FILTER_TYPE_HPF,
        FILTER_TYPE_BPF,
        FILTER_TYPE_NOTCH /*...*/
    } FilterType_t;       // Fits in uint8_t
    typedef enum
    {
        LFO_WAVE_SINE,
        LFO_WAVE_TRIANGLE,
        LFO_WAVE_SAW_UP,
        LFO_WAVE_SAW_DOWN,
        LFO_WAVE_SQUARE,
        LFO_WAVE_RANDOM /*...*/
    } LfoWaveform_t;    // Fits in uint8_t

    //------------------------------------------------------------------------------
    // Function Prototypes (Optional helpers - Implementation in .c file)
    //------------------------------------------------------------------------------

    /**
     * @brief Packs a Set Parameter command message (including command byte) into a buffer.
     *
     * @param buf           Pointer to the output buffer (must be at least 1 + sizeof(SetParamPayload_t) bytes).
     * @param buf_len       Size of the output buffer.
     * @param param_id      The ParamId_t to set.
     * @param param_value   The ParamValue_t data.
     * @return size_t       Number of bytes written to buf (should be 1 + 6 = 7), or 0 on error (e.g., buffer too small).
     */
    size_t i2c_proto_pack_set_param_msg(uint8_t *buf, size_t buf_len, ParamId_t param_id, ParamValue_t param_value);

    /**
     * @brief Unpacks the payload of a Set Parameter command from a buffer.
     * Assumes buffer points to the payload *after* the command byte.
     *
     * @param payload_buf   Pointer to the input payload buffer (must be sizeof(SetParamPayload_t) bytes).
     * @param payload_len   Length of the payload data (should be sizeof(SetParamPayload_t)).
     * @param[out] param_id Pointer to store the unpacked ParamId_t.
     * @param[out] param_value Pointer to store the unpacked ParamValue_t.
     * @return bool         True on success, false on error (e.g., invalid length).
     */
    bool i2c_proto_unpack_set_param_payload(const uint8_t *payload_buf, size_t payload_len, ParamId_t *param_id, ParamValue_t *param_value);

    /**
     * @brief Packs an I2S Configuration command message (including command byte) into a buffer.
     *
     * @param buf Pointer to the output buffer (must be at least 1 + sizeof(I2sConfig_t) bytes).
     * @param buf_len Size of the output buffer.
     * @param config The I2S configuration data.
     * @return size_t Number of bytes written to buf (should be 1 + 4 = 5), or 0 on error.
     */
    size_t i2c_proto_pack_i2s_config_msg(uint8_t *buf, size_t buf_len, const I2sConfig_t *config);

    /**
     * @brief Unpacks the payload of an I2S Configuration command from a buffer.
     * Assumes buffer points to the payload *after* the command byte.
     *
     * @param payload_buf Pointer to the input payload buffer (must be sizeof(I2sConfig_t) bytes).
     * @param payload_len Length of the payload data (should be sizeof(I2sConfig_t)).
     * @param[out] config Pointer to store the unpacked configuration data.
     * @return bool True on success, false on error.
     */
    bool i2c_proto_unpack_i2s_config_payload(const uint8_t *payload_buf, size_t payload_len, I2sConfig_t *config);

#ifdef __cplusplus
}
#endif