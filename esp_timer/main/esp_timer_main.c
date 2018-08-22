/* esp_timer (high resolution timer) example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"

/*
    esp-idf\components\esp32\include\esp_timer.h
    esp-idf\components\esp32\include\esp_sleep.h
*/

static void periodic_timer_callback(void* arg);
static void oneshot_timer_callback(void* arg);

static const char* TAG = "ESP_TIMER";

void app_main()
{
    /* Create two timers:
     * 1. a periodic timer which will run every 0.5s, and print a message
     * 2. a one-shot timer which will fire after 5s, and re-start periodic
     *    timer with period of 1s.
     */

    const esp_timer_create_args_t periodic_timer_args =
    {
            .callback = &periodic_timer_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "periodic"
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    /* The timer has been created but is not running yet */


    const esp_timer_create_args_t oneshot_timer_args =
    {
            .callback = &oneshot_timer_callback,
            /* argument specified here will be passed to timer callback function */
            .arg = (void*) periodic_timer,
            .name = "one-shot"
    };
    esp_timer_handle_t oneshot_timer;
    ESP_ERROR_CHECK(esp_timer_create(&oneshot_timer_args, &oneshot_timer));


    /* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 500000)); // 500ms
    ESP_ERROR_CHECK(esp_timer_start_once(oneshot_timer, 5000000)); // 5s
    ESP_LOGI(TAG, "Started timers, time since boot: %lld us", esp_timer_get_time());


    /* Print debugging information about timers to console every 2 seconds */
    // total 10s
    for (int i = 0; i < 5; ++i)
    {
        ESP_ERROR_CHECK(esp_timer_dump(stdout));
        usleep(2000000);
    }
    /*
    I (268) ESP_TIMER: Started timers, time since boot: 2353 us

    name                period(us)  next_alarm  times_armed  times_triggered(cnt)  total_callback_run_time

    periodic            500000        502341          1          0             0
    one-shot                 0       5002346          1          0             0

    periodic            500000       2502341          1          4           555
    one-shot                 0       5002346          1          0             0

    periodic            500000       4502341          1          8          1070
    one-shot                 0       5002346          1          0             0

    periodic           1000000       7005710          2         11          1458
    one-shot                 0             0          1          1         11994

    periodic           1000000       9005710          2         13          1723
    one-shot                 0             0          1          1         11994
    */


    /* Timekeeping continues in light sleep, and timers are scheduled
     * correctly after light sleep.
     */
    ESP_LOGI(TAG, "Entering light sleep for 0.5s, time since boot: %lld us",
            esp_timer_get_time());

    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(500000)); // 500ms
    esp_light_sleep_start();

    ESP_LOGI(TAG, "Woke up from light sleep, time since boot: %lld us",
                esp_timer_get_time());

    /*
    wakeup by timer

    I (10278) ESP_TIMER: Entering light sleep for 0.5s, time since boot: 10011541 us
    I (10278) ESP_TIMER: Woke up from light sleep, time since boot: 10511997 us
    */


    /* Let the timer run for a little bit more */
    usleep(2000000); // 2s

    /* Clean up and finish the example */
    ESP_ERROR_CHECK(esp_timer_stop(periodic_timer));
    ESP_ERROR_CHECK(esp_timer_delete(periodic_timer));
    ESP_ERROR_CHECK(esp_timer_delete(oneshot_timer));
    ESP_LOGI(TAG, "Stopped and deleted timers");
}

static void periodic_timer_callback(void* arg)
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "Periodic timer called, time since boot: %lld us", time_since_boot);
}

static void oneshot_timer_callback(void* arg)
{
    int64_t time_since_boot = esp_timer_get_time();

    ESP_LOGI(TAG, "One-shot timer called, time since boot: %lld us", time_since_boot);

    esp_timer_handle_t periodic_timer_handle = (esp_timer_handle_t) arg;

    /* To start the timer which is running, need to stop it first */
    ESP_ERROR_CHECK(esp_timer_stop(periodic_timer_handle));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer_handle, 1000000)); // 1s

    time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "Restarted periodic timer with 1s period, time since boot: %lld us",
            time_since_boot);

    /*
    periodic            500000       4502341          1          8          1070
    one-shot                 0       5002346          1          0             0
    I (4768) ESP_TIMER: Periodic timer called, time since boot: 4502367 us
    I (5268) ESP_TIMER: Periodic timer called, time since boot: 5002362 us
    I (5268) ESP_TIMER: One-shot timer called, time since boot: 5002484 us
    I (5268) ESP_TIMER: Restarted periodic timer with 1s period, time since boot: 5005715 us
    I (6268) ESP_TIMER: Periodic timer called, time since boot: 6005732 us
    periodic           1000000       7005710          2         11          1458
    one-shot                 0             0          1          1         11994
    I (7268) ESP_TIMER: Periodic timer called, time since boot: 7005736 us
    I (8268) ESP_TIMER: Periodic timer called, time since boot: 8005732 us
    periodic           1000000       9005710          2         13          1723
    one-shot                 0             0          1          1         11994
    I (9268) ESP_TIMER: Periodic timer called, time since boot: 9005736 us
    */
}
