#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_log.h>
#include <esp_err.h>

#include "esp_audio_mem.h"
#include "smtp_client.h"

static const char *TAG="jpeg_decoding";


extern const uint8_t raw_image_start[] asm("_binary_esp_img_start");
extern const uint8_t raw_image_end[]   asm("_binary_esp_img_end");


/*extern const uint8_t raw_image_start[] asm("_binary_raw_image_start");
extern const uint8_t raw_image_end[]   asm("_binary_raw_image_end");*/

int decode_jpeg(unsigned char *in_buf,
                     unsigned long *in_buf_sz,
                     unsigned char *out_buf
                     );

/*unsigned long in_buf_sz = 12*1024;/*12kB 12 times jpeg image size*/
/*unsigned char in_buf[96 * 96 * 4];*/
unsigned char out_buf[96 * 96 * 12];
/*unsigned long out_buf_sz = 12*1024;*/

static void jpeg_decoder_task(void *pvParameters)
{
    (void) pvParameters;

    /* Fill in image data. Random numbers for now. */
    //for (int i = 0; i < image_width * image_height; i++) {
    //    input_buffer[i] = raw_image_start[i];
    //}

    ESP_LOGI(TAG, "Decoding an image");

    decode_jpeg((unsigned char *) raw_image_start,out_buf,out_buf_sz);


    /* sending an email */
    send_email(out_buf);
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


    xTaskCreateStatic(&jpeg_decoder_task,
                      "jpeg_dec_task",
                      codec_task_stack_size,
                      NULL,
                      5,
                      codec_task_stack,
                      codec_task_buf);

    vTaskDelay(1000);
}