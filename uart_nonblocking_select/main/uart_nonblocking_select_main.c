/* UART Select Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <sys/select.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"

static const char* TAG = "uart_nonblocking_select";

static void uart_select_task()
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_2, 2*1024, 0, 0, NULL, 0);

    //Set UART log level
    esp_log_level_set(TAG, ESP_LOG_INFO);

    uint8_t txData[] = {'e','s','p','3','2'};
    uint8_t rxCount = 0;
    uint8_t rxBuffer[128] = {0,};

    while (1)
    {
        int fd;

        if ((fd = open("/dev/uart/2", O_RDWR)) == -1)
        {
            ESP_LOGE(TAG, "Cannot open UART");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // non-blocking
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);

        // We have a driver now installed so set up the read/write functions to use driver also.
        esp_vfs_dev_uart_use_driver(2); // uart number

        int s;
        fd_set rfds;
        struct timeval tv =
        {
            .tv_sec = 0,
            .tv_usec = 500*1000,
        };

        FD_ZERO(&rfds);

        while (1)
        {
            FD_SET(fd, &rfds);

            s = select(fd + 1, &rfds, NULL, NULL, &tv);

            if (s < 0)
            {
                ESP_LOGE(TAG, "Select failed: errno %d", errno);
                break;
            }
            else if (s == 0)
            {
                //ESP_LOGI(TAG, "Timeout has been reached and nothing has been received");
            }
            else
            {
                if (FD_ISSET(fd, &rfds))
                {
                    char buf;
#if 1
                    while ((s = read(fd, &buf, 1)) > 0)
                    {
                        ESP_LOGI(TAG, "Received: %c", buf);

                        rxBuffer[rxCount] = buf;
                        rxCount += 1;
                        if (rxCount > 127)
                        {
                            rxCount = 0;
                            memset(rxBuffer, 0x00, sizeof(rxBuffer));
                        }
                    }
#endif
#if 0 // not good
                    if (read(fd, &buf, 1) > 0)
                    {
                        ESP_LOGI(TAG, "Received: %c", buf);

                        rxBuffer[rxCount] = buf;
                        rxCount += 1;
                        if (rxCount > 127)
                        {
                            rxCount = 0;
                            memset(rxBuffer, 0x00, sizeof(rxBuffer));
                        }
                    }
                    else
                    {
                        ESP_LOGE(TAG, "UART read error");
                        break;
                    }
#endif
                }
                else
                {
                    ESP_LOGE(TAG, "No FD has been set in select()");
                    break;
                }
            }

            ESP_LOGI(TAG, "rxbuffer [%s][%d]\r\n", rxBuffer, rxCount);

            if (write(fd, &txData, sizeof(txData)) > 0)
            {
                ESP_LOGI(TAG, "write ok");
            }
            else
            {
                ESP_LOGE(TAG, "UART write error");
            }
        }

        close(fd);
    }

    vTaskDelete(NULL);
}

void app_main()
{
    xTaskCreate(uart_select_task, "uart_select_task", 4*1024, NULL, 5, NULL);
}
