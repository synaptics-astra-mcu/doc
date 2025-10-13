# SPI Pre-roll Protocol Specification

## Protocol Overview

This document defines the SPI (Serial Peripheral Interface) transaction format between the Controller and Peripheral devices. The focus is on the doorbell use case to transfer JPEG pre-roll images, including command initiation, image streaming, and completion markers.

## Packet Format
### Controller-to-Peripheral Transfer

The Controller initiates the communication by sending request command to the Peripheral, consisting of a header, an optional payload, and a CRC for integrity checking.

| Field          | Size | Description                                   |
| -------------- | ---- | --------------------------------------------- |
| **CMD_START**  | 2B   | Start of command (e.g., 0x3C3C)               |
| **CMD**        | 2B   | Command ID (e.g., 0x0001 = pre-roll)          |
| **CMD_LEN**    | 2B   | Length of the payload in CMD_DATA (can be 0)  |
| **CMD_CRC**    | 2B   | CRC16 over CMD_DATA, or 0 if CMD_LEN is 0     |
| **CMD_DATA**   | N B  | Optional command payload                      |
| **CMD_END**    | 2B   | End of command (e.g., 0xC3C3)                 |


### Peripheral-to-Controller Transfer

After decoding the request command from Controller, the Peripheral transmits image data frames to the Controller. The transfer consists of an initial header followed by a series of image data frames.

The format includes:

### Start of Transmission Header

This header is sent once at the beginning of the SPI stream.

| Field                   | Size | Description                                    |
| ----------------------- | ---- | ---------------------------------------------- |
| **STREAM_START**        | 2B   | Start tag (e.g., 0xAA55)                       |
| **NUM_FRAMES**          | 2B   | Total number of frames in the transfer session |
| **VERSION**             | 1B   | Protocol version (optional)                    |
| **RESERVED/DATA_TYPE**  | 1B   | Reserved or data type indicator                |

Total size: 6 bytes

### Per-Frame Packet Format

Each image frame follows this structure:

| Field            | Size | Description                          |
| ---------------- | ---- | ------------------------------------ |
| **FRAME_START**  | 2B   | Frame start marker (e.g., 0xA5A5)    |
| **FRAME_INDEX**  | 2B   | Frame number (0 to NUM_FRAMES - 1)   |
| **FRAME_LEN**    | 4B   | Number of bytes in image data        |
| **FRAME_CRC**    | 2B   | CRC16 over the image payload         |
| **IMAGE_DATA**   | N B  | Raw image data (length = FRAME_LEN)  |
| **FRAME_END**    | 2B   | Frame end marker (e.g., 0x5A5A)      |


Total size per frame: 12 bytes + image data (variable length)

### End of Transmission

This marks the end of transmission.

| Field           | Size | Description                              |
| --------------- | ---- | ---------------------------------------- |
| **STREAM_END**  | 2B   | Indicates the end of the stream sequence |

## Transfer Initiation

| Role           | Description                                                                             |
| -------------- | --------------------------------------------------------------------------------------- |
| **Controller** | Sends pre-roll request command (`CMD_ID = 0x0001`) and waits for image frames.          |
| **Peripheral** | Parses the command, prepares pre-roll image frames, and streams them to the Controller. |

## Sender and Receiver Verification

### Controller validates:

- STREAM_START and STREAM_END tags.
- Frame markers (FRAME_START, FRAME_END).
- Frame CRC for image integrity.

### Peripheral validates:

- Proper interpretation of CMD_ID.
- Sending correct NUM_FRAMES.
- Each frame conforms to header/data/footer structure.

## Note

- All multi-byte values use little-endian format.
- CRC16 ensures data integrity for both commands and frames.
- Designed for doorbell use cases (JPEG Pre-roll transfer).
