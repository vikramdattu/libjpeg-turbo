
#pragma once

/* `JPEG_COLOR_SPACE` is Copied from jpeglib.h. `J_COLOR_SPACE` as is */
typedef enum {
  JPEG_COLOR_UNKNOWN,            /* error/unspecified */
  JPEG_COLOR_GRAYSCALE,          /* monochrome */
  JPEG_COLOR_RGB,                /* red/green/blue as specified by the RGB_RED,
                             RGB_GREEN, RGB_BLUE, and RGB_PIXELSIZE macros */
  JPEG_COLOR_YCbCr,              /* Y/Cb/Cr (also known as YUV) */
  JPEG_COLOR_CMYK,               /* C/M/Y/K */
  JPEG_COLOR_YCCK,               /* Y/Cb/Cr/K */
  JPEG_COLOR_EXT_RGB,            /* red/green/blue */
  JPEG_COLOR_EXT_RGBX,           /* red/green/blue/x */
  JPEG_COLOR_EXT_BGR,            /* blue/green/red */
  JPEG_COLOR_EXT_BGRX,           /* blue/green/red/x */
  JPEG_COLOR_EXT_XBGR,           /* x/blue/green/red */
  JPEG_COLOR_EXT_XRGB,           /* x/red/green/blue */
  /* When out_color_space it set to JCS_EXT_RGBX, JCS_EXT_BGRX, JCS_EXT_XBGR,
     or JCS_EXT_XRGB during decompression, the X byte is undefined, and in
     order to ensure the best performance, libjpeg-turbo can set that byte to
     whatever value it wishes.  Use the following colorspace constants to
     ensure that the X byte is set to 0xFF, so that it can be interpreted as an
     opaque alpha channel. */
  JPEG_COLOR_EXT_RGBA,           /* red/green/blue/alpha */
  JPEG_COLOR_EXT_BGRA,           /* blue/green/red/alpha */
  JPEG_COLOR_EXT_ABGR,           /* alpha/blue/green/red */
  JPEG_COLOR_EXT_ARGB,           /* alpha/red/green/blue */
  JPEG_COLOR_RGB565              /* 5-bit red/6-bit green/5-bit blue */
} JPEG_COLOR_SPACE;

/* Error defines */
#define JPEG_SUCCESS            0
#define JPEG_ERR_INVALID_ARG    -1
#define JPEG_ERR_NOMEM          -2
