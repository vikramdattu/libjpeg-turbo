idf_component_register(SRCS "smtp_client.c"
                            "jpeg_decoder.c"
                            "esp_audio_mem.c"
                            "md5.c"
                        INCLUDE_DIRS "."
                        EMBED_TXTFILES server_root_cert.pem
                        EMBED_FILES esp_img_col.jpeg
                        REQUIRES nvs_flash protocol_examples_common mbedtls)
