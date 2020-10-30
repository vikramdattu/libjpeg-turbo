#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_log.h>
#include <esp_err.h>

#include "jpeglib.h"
#include "mbedtls/md5.h"
#include "esp_audio_mem.h"
#include "smtp_client.h"

static const char *TAG="[jpeg_test]";


extern const uint8_t raw_image_start[] asm("_binary_esp_img_col_jpeg_start");
extern const uint8_t raw_image_end[]   asm("_binary_esp_img_col_jpeg_end");

/*extern const uint8_t raw_image_start[] asm("_binary_raw_image_start");
extern const uint8_t raw_image_end[]   asm("_binary_raw_image_end");*/

unsigned long in_buf_sz = 20*1024;/*20kB */
/*unsigned char in_buf[96 * 96 * 4];*/
unsigned char out_buf[96 * 96 * 12];
unsigned long out_buf_sz;
/*unsigned long out_buf_sz = 12*1024;*/

static const char hex[] = "0123456789abcdef";

static void md5_bytes_to_hex(uint8_t *in_buf, uint8_t *out_buf)
{
    for (int i = 0; i < 16; i++) {
        out_buf[i + i] = hex[in_buf[i] >> 4];
        out_buf[i + i + 1] = hex[in_buf[i] & 0x0f];
    }
    out_buf[32] = '\0';
}

static void jpeg_decoder_task(void *pvParameters)
{
    (void) pvParameters;

    ESP_LOGI(TAG, "Decoding an image");

    out_buf_sz = decode_jpeg((unsigned char *)raw_image_start, in_buf_sz, out_buf);
    printf("Out buf size = %ld\n", out_buf_sz);

    /* Calculate MD5sum */
    mbedtls_md5_context ctx;
    mbedtls_md5_init(&ctx);
    mbedtls_md5_starts_ret(&ctx);
    mbedtls_md5_update_ret(&ctx, out_buf, out_buf_sz);
    uint8_t *md5_buf = malloc(16 + 1);
    uint8_t *md5_hex = malloc(32 + 1);
    mbedtls_md5_finish_ret(&ctx, md5_buf);
    //mbedtls_md5_free(&ctx);
    md5_bytes_to_hex(md5_buf, md5_hex);
    printf("md5sum(hex) is: %s\n", md5_hex);

    /* Compare this with reference MD5sum */
    if (strcmp((char *) md5_hex, "<reference md5_sum here!>")) {
        ESP_LOGE(TAG, "Test FAILED");
    } else {
        ESP_LOGI(TAG, "Test PASSED");
    }

    free(md5_buf);
    free(md5_hex);

    /* sending an email */
    //send_email(out_buf,out_buf_sz);
    vTaskDelete(NULL);
}

void app_main()
{
    int codec_task_stack_size = 12 * 1024;
    //init_smtp_client();
    //ESP_LOGI(TAG, "Creating jpeg task");
#if 0
    //StackType_t *codec_task_stack = (StackType_t *) esp_audio_mem_calloc(1, codec_task_stack_size);
    //if(codec_task_stack == NULL) {
    //    ESP_LOGE(TAG, "Error allocating stack for mp3 decoder");
    //    return;
    //}

    //StaticTask_t *codec_task_buf = (StaticTask_t *) heap_caps_calloc(1, sizeof(StaticTask_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    xTaskCreateStatic(&jpeg_decoder_task,
                      "jpeg_dec_task",
                      codec_task_stack_size,
                      NULL,
                      5,
                      codec_task_stack,
                      codec_task_buf);
#else
    xTaskCreate(&jpeg_decoder_task,
                "jpeg_dec_task",
                codec_task_stack_size,
                NULL,
                5,
                NULL);
#endif
    vTaskDelay(1000);
}