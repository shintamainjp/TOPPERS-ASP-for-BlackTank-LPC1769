#ifndef TLV320AIC23B_H
#define TLV320AIC23B_H

/**
 * @brief 0x00: Left line input channel volume control
 * @details
 */
typedef struct {
    uint8_t byte;
    union {
        uint8_t 1:LRS;
        uint8_t 1:LIM;
        uint8_t 1:;
        uint8_t 1:;
        uint8_t 1:LIV4;
        uint8_t 1:LIV3;
        uint8_t 1:LIV2;
        uint8_t 1:LIV1;
        uint8_t 1:LIV0;
    } bit;
} tlv320aic23b_left_line_input_channel_volume_control_t;

void tlv320aic23b_init();
void tlv320aic23b_left_line_input_channel_volume_control_read(
        tlv320aic23b_left_line_input_channel_volume_control_t *p);
void tlv320aic23b_left_line_input_channel_volume_control_write(
        const tlv320aic23b_left_line_input_channel_volume_control_t *p);

#endif

