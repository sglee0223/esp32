
#include <stdio.h>
#include "driver/i2c.h"

#include "ssd1306_lib.h"

static void ssd1306_task(void* arg)
{
    ssd1306_i2c_init();
    ssd1306_reset();
    ssd1306_i2c_scan();

    ssd1306_init();

    ssd1306_set_cursor(0,0);
    ssd1306_puts("012345678901234567890");
    ssd1306_set_cursor(1,0);
    ssd1306_puts("ssd1306 128*64 oled");
    ssd1306_set_cursor(2,0);
    ssd1306_puts("6*8 font > 8row 20col");
    ssd1306_set_cursor(3,0);
    ssd1306_puts("4th");
    ssd1306_set_cursor(4,0);
    ssd1306_puts("5th");
    ssd1306_set_cursor(5,0);
    ssd1306_puts("6th");
    ssd1306_set_cursor(6,0);
    ssd1306_puts("7th");
    ssd1306_set_cursor(7,0);
    ssd1306_puts("8th");

    while(1)
    {
        printf(".\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void app_main()
{
    printf("\n==== Program Start ====\n");

    xTaskCreate(&ssd1306_task, "ssd1306_task", 2048, (void* ) 0, 10, NULL);
}

