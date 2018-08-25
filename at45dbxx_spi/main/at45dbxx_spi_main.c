
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"

#include "at45dbxx.h"

/*
    esp-idf\components\driver\include\driver\spi_master.h
    esp-idf\components\driver\include\driver\spi_common.h
    esp-idf\components\soc\esp32\include\soc\gpio_struct.h
    esp-idf\components\driver\include\driver\gpio.h
*/

void app_main()
{
    esp_err_t ret;
    spi_device_handle_t spi;

    spi_bus_config_t buscfg={
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0, // 0 is default 4096
        .flags = 0,
    };

    spi_device_interface_config_t devcfg={
      .command_bits = 0,
      .address_bits = 0,
      .dummy_bits = 0,
      .mode = 0, // 0 ~ 3, SPI modes '00' (CPOL=CPHA=0) and '11' (CPOL=CPHA=1).
      .duty_cycle_pos = 0, // 0 is default 128, 50% duty
      .cs_ena_pretrans = 0,
      .cs_ena_posttrans = 0,
      .clock_speed_hz = SPI_MASTER_FREQ_20M,
      .input_delay_ns = 0,
      .spics_io_num = -1,// PIN_NUM_CS,
      .flags = 0,
      .queue_size = 3,
      .pre_cb = NULL,
      .post_cb = NULL,
    };

    gpio_set_direction(PIN_NUM_CS, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(PIN_NUM_CS, GPIO_PULLUP_ONLY);

    //Initialize the SPI bus
    ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);

    //Attach the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);

    //Initialize
	at45db_initialize(spi);
	at45db_test(spi);

    while(1)
    {
        vTaskDelay(500 / portTICK_RATE_MS);
    }
}

