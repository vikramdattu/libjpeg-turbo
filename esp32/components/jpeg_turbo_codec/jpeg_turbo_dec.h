#include <stdio.h>
#include <stddef.h>
#include "jpeglib.h"
#include "cderror.h"
#include "jversion.h"
#include "jconfigint.h"

typedef struct decoder_manager {
    unsigned long in_buf_sz; /*Input Buffer Size*/
    unsigned char *out_buf;  /*Output Buffer*/
    unsigned long out_buf_sz; /*Output Buffer Size*/
    JDIMENSION output_width; /*Decoded Image Width*/
    JDIMENSION output_height; /*Decoded Image Height*/
    int out_col_comp; /*Number of Color Components in Decoded Image*/
    J_COLOR_SPACE out_color_space; /*Decoded Image ColorSpace*/
    unsigned char *in_buf; /*Pointer to Embedded Image's Start*/
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
