#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>
#include <esp_err.h>
#include "esp_audio_mem.h"
#include "smtp_client.h"
#include "jpeg_turbo_dec.h"

#define INPUT_BUF_SIZE (20 * 1024)
#define INP_IMG_WIDTH 94
#define INP_IMG_HEIGHT 94

static const char *TAG = "[jpeg_decoder]";

extern uint8_t raw_image_start[] asm("_binary_esp_img_col_jpeg_start");
extern uint8_t raw_image_end[]   asm("_binary_esp_img_col_jpeg_end");

static void jpeg_decoder_task(void *pvParameters)
{
    (void) pvParameters;
    int ret;
    decoder_context dec;
    dec.in_buf_sz = INPUT_BUF_SIZE;
    dec.out_buf = malloc(INP_IMG_WIDTH * INP_IMG_HEIGHT * 12);
    dec.out_color_space = JPEG_COLOR_RGB;
    dec.in_buf = raw_image_start;
    ESP_LOGI(TAG, "Decoding an image");
    ret = decode_jpeg(&dec);

    if (ret == JPEG_ERR_INVALID_ARG) {
        ESP_LOGE(TAG, "Invalid Arguments Passed");
        goto task_exit;
    }

    if (ret == JPEG_ERR_NOMEM) {
        ESP_LOGE(TAG, "Out of memory");
        goto task_exit;
    }

    if (ret == JPEG_SUCCESS) {
        ESP_LOGI(TAG, "Success");
    }
    /* sending an email */
    send_email(dec.out_buf, dec.out_buf_sz);

task_exit:
    free(dec.out_buf);
    vTaskDelete(NULL);
}

void app_main()
{
    int codec_task_stack_size = 12 * 1024;
    init_smtp_client();

    xTaskCreate(&jpeg_decoder_task,
                "jpeg_dec_task",
                codec_task_stack_size,
                NULL,
                5,
                NULL);

    vTaskDelay(1000);
}

