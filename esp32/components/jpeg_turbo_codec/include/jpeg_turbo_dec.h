#include <stdio.h>
#include <stddef.h>
#include "jpeg_turbo_common.h"

typedef struct decoder_manager {
    unsigned long in_buf_sz;        /* Input Buffer Size */
    unsigned char *out_buf;         /* Output Buffer */
    unsigned long out_buf_sz;       /* Output Buffer Size */
    unsigned int output_width;      /* Decoded Image Width */
    unsigned int output_height;     /* Decoded Image Height */
    int out_col_comp;               /* Number of Color Components in Decoded Image */
    JPEG_COLOR_SPACE out_color_space;  /* Decoded Image ColorSpace */
    unsigned char *in_buf;          /* Pointer to Embedded Image's Start */
} decoder_context;

/**
 * @brief      Decodes Jpeg Image colored/greyscale
 *
 * @param[in]  pointer   decoder_context structure pointer
 *
 * @return     0 When Success,
 *            -1 When Invalid arguments passed,
 *            -2 When Out of memory
 */
int decode_jpeg(decoder_context *mgr);
