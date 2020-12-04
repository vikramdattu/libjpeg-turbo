#include "jpeg_turbo_enc.h"
#include <esp_log.h>

#include "jpeglib.h"
#include "cderror.h"

static const char *TAG = "jpeg_enc";

static const char *const cdjpeg_message_table[] = {
    NULL
};

int encode_image(encoder_context *mgr)
{
    if (mgr->output_buffer == NULL || mgr->image_height == 0 ||
        mgr->image_width == 0 || mgr->in_col_components == 0 ||
        mgr->in_col_space == JPEG_COLOR_UNKNOWN || mgr->inp_buf == NULL ||
        mgr->out_buf_sz == 0) {
        return JPEG_ERR_INVALID_ARG;
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
#ifdef PROGRESS_REPORT
    struct cdjpeg_progress_mgr progress;
#endif

    /* Initialize the JPEG compression object with default error handling. */
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    /* Add some application-specific error messages (from cderror.h) */
    jerr.addon_message_table = cdjpeg_message_table;
    jerr.first_addon_message = JMSG_FIRSTADDONCODE;
    jerr.last_addon_message = JMSG_LASTADDONCODE;

    /* Initialize JPEG parameters.
     * Much of this may be overridden later.
     * In particular, we don't yet know the input file's color space,
     * but we need to provide some value for jpeg_set_defaults() to work.
     */

    cinfo.image_width = mgr->image_width;
    cinfo.image_height = mgr->image_height;
    cinfo.input_components = mgr->in_col_components;
    cinfo.in_color_space = mgr->in_col_space; /* arbitrary guess */
    jpeg_set_defaults(&cinfo);

    if (mgr->quality > 100) {
        mgr->quality = 75;
        ESP_LOGW(TAG, "Quality param out of range [0, 100]! Setting it to 75...");
    }
    if (mgr->quality != 0) {
        jpeg_set_quality(&cinfo, mgr->quality, TRUE);
    }

#ifdef PROGRESS_REPORT
    start_progress_monitor((j_common_ptr)&cinfo, &progress);
#endif

    /* Specify data destination for compression */
    jpeg_mem_dest(&cinfo, &mgr->output_buffer, &mgr->out_buf_sz);

    /* Start compressor */
    jpeg_start_compress(&cinfo, TRUE);

    unsigned char *buf[3];
    while (cinfo.next_scanline < (cinfo.image_height)) {
        buf[0] = mgr->inp_buf + (cinfo.image_width * cinfo.next_scanline * cinfo.input_components);
        if(cinfo.next_scanline >= cinfo.image_height) {
            return JPEG_ERR_NOMEM;
        }
        jpeg_write_scanlines(&cinfo, (JSAMPARRAY) buf, 1);
    }

    if(cinfo.next_scanline < cinfo.image_height) {
        return JPEG_ERR_NOMEM;
    }

    /* Finish compression and release memory */
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

#ifdef PROGRESS_REPORT
    end_progress_monitor((j_common_ptr)&cinfo);
#endif

    return JPEG_SUCCESS;
}
