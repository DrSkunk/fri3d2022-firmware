#include "eyeball.h"

#include <stdio.h>
#include <string.h>

#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "esp32/ulp.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "graphics_wrapper.h"
#include "hardware.h"
#include "nvs.h"
#include "pax_gfx.h"
#include "sdkconfig.h"
#include "soc/rtc.h"
#include "wifi_connect.h"
#include "ws2812.h"

#define SLEEP_DELAY 10000
static const char *TAG = "eye";

static void cycle_leds(){
        int hue = esp_random() & 255;
        pax_col_t color = pax_col_hsv(hue, 255 /*saturation*/, 255 /*brighness*/);
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = (color >> 0) & 0xFF;

        uint8_t led_buffer[50 * 3];
        for (uint8_t i = 0; i < sizeof(led_buffer); i += 3) {
            led_buffer[i]     = g;
            led_buffer[i + 1] = r;
            led_buffer[i + 2] = b;
        }
}

static void show_eye() {
    pax_buf_t        *pax_buffer        = get_pax_buffer();
    pax_background(pax_buffer, 0x000000);
    // Allen, add eyes
    display_flush();
}

void show_eyeball() {
    input_message_t msg;
    bool                   quit = false;
    while (!quit) {
        show_eye();
        if (xQueueReceive(get_input_queue(), &msg, pdMS_TO_TICKS(SLEEP_DELAY + 10))) {
            if (msg.state) {
                switch (msg.input) {
                    case INPUT_TOUCH0:
                        quit = true;
                        break;
                    case INPUT_TOUCH1:
                        // Cycle color of LEDs
                        cycle_leds();
                        break;
                    case INPUT_TOUCH2:
                        // blink eye
                        break;
                    default:
                        break;
                }
            }
            ESP_LOGI(TAG, "Recheduled sleep in %d millis", SLEEP_DELAY);
        }
    }

    uint8_t led_buffer[50 * 3] = {0};
    ws2812_send_data(led_buffer, sizeof(led_buffer));
}
