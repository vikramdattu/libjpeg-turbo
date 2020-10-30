#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_log.h>
#include <esp_err.h>

#include "esp_audio_mem.h"
#include "smtp_client.h"
#include "../../md5/md5.h"
//#include "platform.h"
//#include "platform_util.h"
//#include "config.h"


static const char *TAG="jpeg_decoding";


extern const uint8_t raw_image_start[] asm("_binary_esp_img_col_jpeg_start");
extern const uint8_t raw_image_end[]   asm("_binary_esp_img_col_jpeg_end");


/*extern const uint8_t raw_image_start[] asm("_binary_raw_image_start");
extern const uint8_t raw_image_end[]   asm("_binary_raw_image_end");*/

int decode_jpeg(unsigned char *in_buf,unsigned long in_buf_sz,unsigned char *out_buf);

unsigned long in_buf_sz = 20*1024;/*20kB */
/*unsigned char in_buf[96 * 96 * 4];*/
unsigned char out_buf[96 * 96 * 12];
unsigned long out_buf_sz;
/*unsigned long out_buf_sz = 12*1024;*/
char *buf;

static char *MD5End(MD5_CTX *ctx, char *buf)
{
  int i;
  unsigned char digest[16];
  static const char hex[] = "0123456789abcdef";

  if (!buf)
    buf = malloc(2 * 16 + 1);
  if (!buf)
    return 0;
  MD5Final(digest, ctx);
  for (i = 0; i < 16; i++) {
    buf[i + i] = hex[digest[i] >> 4];
    buf[i + i + 1] = hex[digest[i] & 0x0f];
  }
  buf[i + i] = '\0';
  return buf;
}

static void jpeg_decoder_task(void *pvParameters)
{
    (void) pvParameters;

    /* Fill in image data. Random numbers for now. */
    //for (int i = 0; i < image_width * image_height; i++) {
    //    input_buffer[i] = raw_image_start[i];
    //}

    ESP_LOGI(TAG, "Decoding an image");

    out_buf_sz = decode_jpeg((unsigned char *)raw_image_start,in_buf_sz,out_buf);
    printf("Out buf size = %ld   ",out_buf_sz);
    /* sending an email */
    MD5_CTX ctx;
    MD5Init(&ctx);
    MD5Update(&ctx,out_buf,out_buf_sz);
    buf=MD5End(&ctx, (char *)out_buf);

    send_email(out_buf,out_buf_sz);
    vTaskDelete(NULL);
}


void app_main()
{
    int codec_task_stack_size = 12 * 1024;
    init_smtp_client();
   // StackType_t *codec_task_stack = (StackType_t *) esp_audio_mem_calloc(1, codec_task_stack_size);
   // if(codec_task_stack == NULL) {
   //     ESP_LOGE(TAG, "Error allocating stack for mp3 decoder");
   //     return;
   // }

   // StaticTask_t *codec_task_buf = (StaticTask_t *) heap_caps_calloc(1, sizeof(StaticTask_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

   //ESP_LOGI(TAG, "Creating jpeg task");


#if 0
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