#include <stdio.h>
#include <stddef.h>
#include "jpeg_turbo_common.h"

typedef struct encoder_manager {
    unsigned int image_height;      /* Input Image Height */
    unsigned int image_width;       /* Input Image Width */
    unsigned long out_buf_sz;       /* Output Buffer Size */
    unsigned char *output_buffer;   /* Output Buffer */
    JPEG_COLOR_SPACE in_col_space;     /* Input Image Colorspace */
    unsigned char *inp_buf;         /* Input Image pointer */
    int in_col_components;          /* Number of color components in Input Image */
    unsigned int quality;           /* Quality of output image , Range:[0-100] */
} encoder_context;

/**
 * @brief      Encodes Raw Colored/Greyscale Images
 *
 * @param[in]  pointer   encoder_context structure pointer
 *
 * @return     0 When Success,
 *            -1 When Invalid arguments passed,
 *            -2 When Out of memory
 */
int encode_image(encoder_context *mgr);
