/**
 * Copyright (c) 2022 Nicolai Electronics
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>
#include <driver/spi_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

// Registers
#define ST7789V_NOP       0x00
#define ST7789V_SWRESET   0x01
#define ST7789V_RDDID     0x04
#define ST7789V_RDDST     0x09
#define ST7789V_RDDPM     0x0A
#define ST7789V_RDDMADCTL 0x0B
#define ST7789V_RDDCOLMOD 0x0C
#define ST7789V_RDDIM     0x0D
#define ST7789V_RDDSM     0x0E
#define ST7789V_RDSDR     0x0F //ST7789V
#define ST7789V_SLPIN     0x10
#define ST7789V_SLPOUT    0x11
#define ST7789V_PTLON     0x12
#define ST7789V_NORON     0x13
#define ST7789V_INVOFF    0x20
#define ST7789V_INVON     0x21
#define ST7789V_GAMSET    0x26
#define ST7789V_DISPOFF   0x28
#define ST7789V_DISPON    0x29
#define ST7789V_CASET     0x2A
#define ST7789V_RASET     0x2B
#define ST7789V_RAMWR     0x2C
#define ST7789V_RAMRD     0x2E
#define ST7789V_PTLAR     0x30
#define ST7789V_VSCRDEF   0x33 //ST7789V
#define ST7789V_TEOFF     0x34
#define ST7789V_TEON      0x35
#define ST7789V_MADCTL    0x36
#define ST7789V_VSCSAD    0x37 //ST7789V
#define ST7789V_IDMOFF    0x38
#define ST7789V_IDMON     0x39
#define ST7789V_COLMOD    0x3A
#define ST7789V_WRMEMC    0x3C //ST7789V
#define ST7789V_RDMEMC    0x3E //ST7789V
#define ST7789V_STE       0x44 //ST7789V
#define ST7789V_GSCAN     0x45 //ST7789V
#define ST7789V_WRDISBV   0x51 //ST7789V
#define ST7789V_RDDISBV   0x52 //ST7789V
#define ST7789V_WRCTRLD   0x53 //ST7789V
#define ST7789V_RDCTRLD   0x54 //ST7789V
#define ST7789V_WRCACE    0x55 //ST7789V
#define ST7789V_RDCABC    0x56 //ST7789V
#define ST7789V_WRCABCMB  0x5E //ST7789V
#define ST7789V_RDCABCMB  0x5F //ST7789V
#define ST7789V_RDABCSDR  0x68 //ST7789V
#define ST7789V_RAMCTRL   0xB0 //ST7789V
#define ST7789V_RGBCTRL   0xB1 //ST7789V
#define ST7789V_PORCTRL   0xB2 //ST7789V
#define ST7789V_FRCTRL1   0xB3 //ST7789V
#define ST7789V_PARCTRL   0xB5 //ST7789V
#define ST7789V_GCTRL     0xB7 //ST7789V
#define ST7789V_GTADJ     0xB8 //ST7789V
#define ST7789V_DGMEN     0xBA //ST7789V
#define ST7789V_VCOMS     0xBB //ST7789V
#define ST7789V_LCMCTRL   0xC0 //ST7789V
#define ST7789V_IDSET     0xC1 //ST7789V
#define ST7789V_VDVVRHEN  0xC2 //ST7789V
#define ST7789V_VRHS      0xC3 //ST7789V
#define ST7789V_VDVSET    0xC4 //ST7789V
#define ST7789V_VCMOFSET  0xC5 //ST7789V
#define ST7789V_FCTR2     0xC6 //ST7789V
#define ST7789V_CABCCTRL  0xC7 //ST7789V
#define ST7789V_REGSEL1   0xC8 //ST7789V
#define ST7789V_REGSEL2   0xCA //ST7789V
#define ST7789V_PWMFRSEL  0xCC //ST7789V
#define ST7789V_PWCTRL1   0xD0 //ST7789V
#define ST7789V_VAPVANEN  0xD2 //ST7789V
#define ST7789V_CMD2EN    0xDF //ST7789V
#define ST7789V_PVGAMCTRL 0xE0 //ST7789V
#define ST7789V_NVGAMCTRL 0xE1 //ST7789V
#define ST7789V_DGMLUTR   0xE2 //ST7789V
#define ST7789V_DGMLUTB   0xE3 //ST7789V
#define ST7789V_GATECTRL  0xE4 //ST7789V
#define ST7789V_SPI2EN    0xE7 //ST7789V
#define ST7789V_PWCTRL2   0xE8 //ST7789V
#define ST7789V_EQCTRL    0xE9 //ST7789V
#define ST7789V_PROMCTRL  0xEC //ST7789V
#define ST7789V_PROMEN    0xFA //ST7789V
#define ST7789V_NVMSET    0xFC //ST7789V
#define ST7789V_PROMACT   0xFE //ST7789V

typedef struct ST7789V {
    // Pins
    int spi_bus;
    int pin_cs;
    int pin_dcx;
    int pin_reset;
    // Configuration
	uint16_t offset_x;
	uint16_t offset_y;
	uint16_t width;
	uint16_t height;
    uint32_t spi_speed;
    uint32_t spi_max_transfer_size;
	bool reset_open_drain;
    // Internal state
    spi_device_handle_t spi_device;
    bool dc_level;
	uint8_t* internal_buffer;
    // Mutex
    SemaphoreHandle_t mutex;
} ST7789V;

esp_err_t st7789v_init(ST7789V* device);
esp_err_t st7789v_deinit(ST7789V* device);

esp_err_t st7789v_set_backlight(ST7789V* device, bool state);

esp_err_t st7789v_write(ST7789V* device, const uint8_t *data);
esp_err_t st7789v_write_partial(ST7789V* device, const uint8_t *buffer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

#ifdef __cplusplus
}
#endif //__cplusplus