# I2C Communication Protocol for ESP32-S3 Modular Synth

This repository/component defines the shared I2C communication protocol used between the Central Controller (I2C Master) and various Synthesizer Modules (I2C Slaves) in the ESP32-S3 Modular Synthesizer project.

## Purpose

To provide a standardized way for the Central Controller to configure modules, send commands, set parameters, and query status over the main (multiplexed) I2C bus. Both the master (`i2c_manager` component in Central Controller) and slave implementations (in each module's firmware) rely on this definition.

## Key Protocol Features

* **Command/Register Structure:** Master writes a command/register byte, optionally followed by data payload. Reads involve the master writing the target register address first, then initiating a read.
* **Common Registers:** Defined registers for reading standard information like `REG_COMMON_MODULE_TYPE`, `REG_COMMON_FIRMWARE_VERSION`, `REG_COMMON_STATUS`.
* **Common Commands:** Defined commands for actions like `CMD_COMMON_RESET`, `CMD_COMMON_SAVE_SETTINGS`.
* **I2S Configuration:** A dedicated command (`REG_COMMON_I2S_CONFIG`) with a structured payload (`I2sConfig_t`) for assigning TDM slots.
* **Parameter Setting:** A generic command (`REG_COMMON_SET_PARAM`) uses a `ParamId_t` (uint16_t) to identify the target parameter and a `ParamValue_t` (4-byte union) to carry the value, allowing flexible data types (uint8/16/32, int8/16/32).
* **Extensibility:** Parameter IDs (`ParamId_t`) are organized in ranges to allow adding new modules and parameters easily.

## Files

* **`include/module_i2c_proto.h`**: The main header file containing all definitions (enums, structs, constants, function prototypes for helpers). This is the primary file to include.
* **`module_i2c_proto.c`**: (Optional) Implementation for helper functions (e.g., packing/unpacking message payloads).

## Data Types

* Parameters are identified by `ParamId_t` (uint16_t).
* Parameter values are sent using the `ParamValue_t` union (4 bytes), allowing interpretation as `uint32_t`, `int32_t`, `uint16_t[2]`, `int16_t[2]`, or `uint8_t[4]`. The specific interpretation is determined by the `ParamId_t` (documented in comments within the header).

## Usage

This component is a dependency for:

1.  The **Central Controller** firmware (specifically its `i2c_manager` component).
2.  The firmware for **all slave modules** (Oscillator, Filter, FX, LFO, Mixer, IO Modules, etc.).

Dependency management should be handled via the ESP-IDF Component Manager, referencing this repository.

```c
// Example include in both master and slave code
#include "module_i2c_proto.h"
