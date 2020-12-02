idf_component_register(SRCS "smtp_client.c"
                            "jpeg_test.c"
                            "esp_audio_mem.c"
                        INCLUDE_DIRS "."
                        EMBED_TXTFILES server_root_cert.pem
                        EMBED_FILES test2_100.jpeg
                        REQUIRES nvs_flash protocol_examples_common mbedtls enc_dec_files)
