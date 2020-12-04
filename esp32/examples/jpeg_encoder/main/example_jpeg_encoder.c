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
#include "jpeg_turbo_enc.h"

#define IMAGE_HEIGHT 94
#define IMAGE_WIDTH 94
#define QUALITY 50

static const char *TAG = "jpeg_enc";

extern uint8_t raw_image_start[] asm("_binary_raw_img_start");
extern uint8_t raw_image_end[]   asm("_binary_raw_img_end");

static void jpeg_encoder_task(void *pvParameters)
{
    (void) pvParameters;
    int ret;
    ESP_LOGI(TAG, "Encoding an image");
    encoder_context enc;
    enc.image_height = IMAGE_HEIGHT;
    enc.image_width = IMAGE_WIDTH;
    enc.out_buf_sz = IMAGE_WIDTH * IMAGE_HEIGHT * 4;
    enc.in_col_components = 3;
    enc.in_col_space = JPEG_COLOR_RGB;
    enc.output_buffer = malloc(IMAGE_HEIGHT * IMAGE_WIDTH * 4);
    enc.inp_buf = raw_image_start;
    enc.quality = QUALITY;

    ret = encode_image(&enc);
    if (ret == JPEG_ERR_INVALID_ARG) {
        ESP_LOGE(TAG, "Invalid Arguments Passed\tAborting program");
        goto task_exit;
    }

    if (ret == JPEG_ERR_NOMEM) {
        ESP_LOGE(TAG, "Out of memory\tAborting program");
        goto task_exit;
    }

    if (ret == JPEG_SUCCESS) {
        ESP_LOGI(TAG, "Success");
    }
    /* sending an email */
    send_email(enc.output_buffer, enc.out_buf_sz);

task_exit:
    free(enc.output_buffer);
    vTaskDelete(NULL);
}

void app_main()
{
    int codec_task_stack_size = 12 * 1024;
    init_smtp_client();

    xTaskCreate(&jpeg_encoder_task,
                "jpeg_enc_task",
                codec_task_stack_size,
                NULL,
                5,
                NULL);

    vTaskDelay(1000);
}
