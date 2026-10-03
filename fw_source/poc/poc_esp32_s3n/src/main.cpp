/*************************************************************************************************
 * Diego Minguzzi
 *************************************************************************************************/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"

static const char *TAG = "poc";
#define LED_GPIO_PIN    48   // Common onboard RGB LED pin for many ESP32-S3 boards (e.g., DevKitC)
#define LED_NUM_PIXELS  1    // Number of LEDs

extern "C" {
  void app_main();
}

led_strip_handle_t configureLed(void)
{
  led_strip_config_t stripConfig{};
  stripConfig.strip_gpio_num = LED_GPIO_PIN;
  stripConfig.max_leds = LED_NUM_PIXELS;

  led_strip_rmt_config_t rmtConfig{};
  rmtConfig.resolution_hz = 10 * 1000 * 1000; // 10MHz
  rmtConfig.flags.with_dma = false;

  led_strip_handle_t ledStrip{};

  ESP_ERROR_CHECK(led_strip_new_rmt_device( &stripConfig,
                                            &rmtConfig,
                                            &ledStrip));
  return ledStrip;
}

void app_main()
{
  led_strip_handle_t ledStrip= configureLed();
  uint32_t ledBrightness{ 10};
  while (true)
  {
    ESP_LOGI(TAG, "Cycle");
    led_strip_set_pixel(ledStrip, 0, ledBrightness, 0, 0);
    led_strip_refresh(ledStrip);
    vTaskDelay( pdMS_TO_TICKS( 1000 ));

    led_strip_set_pixel(ledStrip, 0, 0, ledBrightness, 0);
    led_strip_refresh(ledStrip);
    vTaskDelay( pdMS_TO_TICKS( 1000 ));

    led_strip_set_pixel(ledStrip, 0, 0, 0, ledBrightness);
    led_strip_refresh(ledStrip);
    vTaskDelay( pdMS_TO_TICKS( 1000 ));
  }
}