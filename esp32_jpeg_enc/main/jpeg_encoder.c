/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_log.h>
#include <esp_err.h>

#include "esp_audio_mem.h"

static const char *TAG = "jpeg_enc";

/* Introduced by ESP. Encode raw mono-chrome image */
int encode_greyscale(unsigned char *in_buf, unsigned char *out_buf,
                     unsigned long out_buf_sz,
                     int image_width, int image_height);

const int image_height = 96;
const int image_width = 96;
int out_buf_sz = image_width * image_height;
unsigned char input_buffer[96 * 96];
unsigned char output_buffer[96 * 96];

static void jpeg_encoder_task(void *pvParameters)
{
    (void) pvParameters;

    ESP_LOGI(TAG, "Encoding an image");

    encode_greyscale(input_buffer, output_buffer, out_buf_sz, image_width, image_height);

    ESP_LOGI(TAG, "Encoded an image");

    vTaskDelete(NULL);
}

void app_main()
{
    int codec_task_stack_size = 4 * 1024;
    StackType_t *codec_task_stack = (StackType_t *) esp_audio_mem_calloc(1, codec_task_stack_size);
    if(codec_task_stack == NULL) {
        ESP_LOGE(TAG, "Error allocating stack for mp3 decoder");
        return;
    }
    StaticTask_t *codec_task_buf = (StaticTask_t *) heap_caps_calloc(1, sizeof(StaticTask_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

    ESP_LOGI(TAG, "Creating jpeg task");

    xTaskCreateStatic(&jpeg_encoder_task,
                      "jpeg_enc_task",
                      codec_task_stack_size,
                      NULL,
                      5,
                      codec_task_stack,
                      codec_task_buf);

    vTaskDelay(1000);
}
