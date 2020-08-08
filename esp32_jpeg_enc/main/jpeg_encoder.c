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
#include "smtp_client.h"

static const char *TAG = "jpeg_enc";

extern const uint8_t raw_image_start[] asm("_binary_raw_image_start");
extern const uint8_t raw_image_end[]   asm("_binary_raw_image_end");

/* Introduced by ESP. Encode raw mono-chrome image */
int encode_greyscale(unsigned char *in_buf, unsigned char *out_buf,
                     unsigned long *out_buf_sz,
                     int image_width, int image_height);

const int image_height = 96;
const int image_width = 96;
unsigned long out_buf_sz = image_width * image_height * 4;
unsigned char input_buffer[96 * 96];
unsigned char output_buffer[96 * 96 * 4];

static void jpeg_encoder_task(void *pvParameters)
{
    (void) pvParameters;

    /* Fill in image data. Random numbers for now. */
    //for (int i = 0; i < image_width * image_height; i++) {
    //    input_buffer[i] = raw_image_start[i];
    //}

    ESP_LOGI(TAG, "Encoding an image");

    encode_greyscale((unsigned char *) raw_image_start, output_buffer,
                     &out_buf_sz, image_width, image_height);

    ESP_LOGI(TAG, "Encoded an image. out_size = %lu", out_buf_sz);

    /* sending an email */
    send_email(output_buffer, out_buf_sz);
    vTaskDelete(NULL);
}

void app_main()
{
    int codec_task_stack_size = 12 * 1024;
    StackType_t *codec_task_stack = (StackType_t *) esp_audio_mem_calloc(1, codec_task_stack_size);
    if(codec_task_stack == NULL) {
        ESP_LOGE(TAG, "Error allocating stack for mp3 decoder");
        return;
    }
    StaticTask_t *codec_task_buf = (StaticTask_t *) heap_caps_calloc(1, sizeof(StaticTask_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    init_smtp_client();
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
