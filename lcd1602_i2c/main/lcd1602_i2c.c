
#include <stdio.h>
#include "driver/i2c.h"

#include "lcd1602_lib.h"

static void lcd1602_task(void* arg)
{
    lcd1602_i2c_init();
	i2c_scan();
	lcd1602_Init();

	// set address to 0x00
	lcd1602_SendCommand(0x80);
	lcd1602_SendString(" Using 1602 LCD");

	// set address to 0x40
	lcd1602_SendCommand(0xC0);
	lcd1602_SendString("  over I2C bus");

    while(1)
    {
        printf(".\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void app_main()
{
    printf("\n==== Program Start ====\n");

    xTaskCreate(&lcd1602_task, "lcd1602_task", 2048, (void* ) 0, 10, NULL);
}

