idf_component_register(SRCS "smtp_client.c"
                            "jpeg_encoder.c"
                            "esp_audio_mem.c"
                        INCLUDE_DIRS "."
                        EMBED_TXTFILES server_root_cert.pem raw_image
                        REQUIRES nvs_flash protocol_examples_common mbedtls)
