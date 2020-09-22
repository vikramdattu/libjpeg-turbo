#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_log.h>
#include <esp_err.h>

#include "esp_audio_mem.h"
#include "smtp_client.h"

static const char *TAG="jpeg_decoding";

/*extern const uint8_t raw_image_start[] asm("_binary_raw_image_start");
extern const uint8_t raw_image_end[]   asm("_binary_raw_image_end");*/

int decode_greyscale(unsigned char *in_buf,
                     unsigned long *in_buf_sz,
                     );

unsigned long in_buf_sz = 96 * 96 * 4;
unsigned char in_buf[96 * 96 * 4];

static void jpeg_decoder_task(void *pvParameters)
{
    (void) pvParameters;

    /* Fill in image data. Random numbers for now. */
    //for (int i = 0; i < image_width * image_height; i++) {
    //    input_buffer[i] = raw_image_start[i];
    //}

    ESP_LOGI(TAG, "Decoding an image");

    decode_greyscale(in_buf,in_buf_sz);


    /* sending an email */
   /* send_email(output_buffer, out_buf_sz); <- what to send to mail? */
    vTaskDelete(NULL);
}

void app main()
{
    xTaskCreateStatic(&jpeg_decoder_task,
                      "jpeg_denc_task",
                      NULL,/* stack size required?*/
                      NULL,
                      5,
                     /* codec_task_stack,
                      codec_task_buf*/);

    vTaskDelay(1000);
}