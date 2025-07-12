#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "u8g2_esp32_hal.h"
#include <u8g2.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 128
#define HEIGHT 64
#define STAR_COUNT 50

u8g2_t u8g2;
int stars_x[STAR_COUNT], stars_y[STAR_COUNT];

void init_stars() {
    for(int i=0;i<STAR_COUNT;i++){
        stars_x[i] = rand()%WIDTH;
        stars_y[i] = rand()%HEIGHT;
    }
}

void draw_stars() {
    u8g2_ClearBuffer(&u8g2);
    for(int i=0;i<STAR_COUNT;i++){
        if(rand()%10 > 7)  
            u8g2_DrawBox(&u8g2, stars_x[i], stars_y[i],2,2 );
    }
    u8g2_SendBuffer(&u8g2);
}

void app_main() {
    srand(time(NULL));

    u8g2_esp32_hal_t hal = U8G2_ESP32_HAL_DEFAULT;
    hal.bus.i2c.sda = GPIO_NUM_21;
    hal.bus.i2c.scl = GPIO_NUM_22;
    u8g2_esp32_hal_init(hal);

    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
        &u8g2, U8G2_R0,
        u8g2_esp32_i2c_byte_cb,
        u8g2_esp32_gpio_and_delay_cb
    );
    u8x8_SetI2CAddress(&u8g2.u8x8, 0x3C<<1);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    init_stars();

    while(1) {
        draw_stars();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
