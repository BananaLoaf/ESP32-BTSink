#include <Arduino.h>
#include <esp_gap_bt_api.h>
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"
#include "driver/i2s.h"

#include <../lib/btAudio/src/btAudio.h>


#define SINK_NAME "BTSink"
#define MAIN_CORE 1
#define RTC_CORE 0


BTSink audio = BTSink(SINK_NAME);

TaskHandle_t BTSinkTask;
TaskHandle_t LEDControllerTask;

[[noreturn]] void start_bt_sink(void *parameter) {
    Serial.printf("Starting BT Sink on core %i", xPortGetCoreID());
    Serial.println();

    audio.begin();

    int bck = 26;
    int ws = 22;
    int dout = 25;
    audio.i2s(bck, dout, ws);

    for(;;){vTaskDelay(100);}
}

[[noreturn]] void led_control(void *parameter) {
    Serial.printf("Starting LED Control on core %i", xPortGetCoreID());
    Serial.println();

    for (;;) {
        if (audio.is_connected()) {
            digitalWrite(LED_BUILTIN, HIGH);
            vTaskDelay(1000);
        } else {
            digitalWrite(LED_BUILTIN, HIGH);
            vTaskDelay(1000);
            digitalWrite(LED_BUILTIN, LOW);
            vTaskDelay(1000);
        }
    }
}


void setup() {
    Serial.begin(115200);
    xTaskCreatePinnedToCore(start_bt_sink, "BT Sink", 38400, NULL, 2, &BTSinkTask, MAIN_CORE);

    pinMode(LED_BUILTIN, OUTPUT);
    xTaskCreatePinnedToCore(led_control, "LED Control", 4096, NULL, 1, &LEDControllerTask, RTC_CORE);
}

void loop() {vTaskDelay(1000);}
