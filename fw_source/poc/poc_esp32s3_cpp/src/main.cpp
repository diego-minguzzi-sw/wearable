/*************************************************************************************************
 * Diego Minguzzi
 *************************************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "poc";

extern "C" {
  void app_main();
}

//-------------------------------------------------------------------------------------------------
void app_main()
{
    while (true)
  {
    ESP_LOGI(TAG, "Cycle executed.");
    vTaskDelay( pdMS_TO_TICKS( 1000 ));
  }
}