idf_component_register(SRCS "jpeg_turbo_dec.c"
			    "jpeg_turbo_enc.c"
                       INCLUDE_DIRS "."
                       REQUIRES nvs_flash protocol_examples_common mbedtls libjpeg-turbo)
