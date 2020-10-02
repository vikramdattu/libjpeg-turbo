idf_component_register(SRCS "smtp_client.c"
                            "jpeg_decoder.c"
                            "esp_audio_mem.c"
                        INCLUDE_DIRS "."
                        EMBED_FILES server_root_cert.pem esp_img.jpeg
                        REQUIRES nvs_flash protocol_examples_common mbedtls)
