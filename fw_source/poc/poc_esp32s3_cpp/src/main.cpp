/*************************************************************************************************
 * Diego Minguzzi
 *************************************************************************************************/
#include "esp_flash.h"
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
  uint32_t size{};
  esp_flash_get_size(NULL, &size);
  ESP_LOGI(TAG, "Flash size: %lu bytes", size);
  while (true)
  {
    ESP_LOGI(TAG, "Cycle executed.");
    vTaskDelay( pdMS_TO_TICKS( 1000 ));
  }
}