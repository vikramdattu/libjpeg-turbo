idf_component_register(SRCS "smtp_client.c"
                            "jpeg_test.c"
                            "esp_audio_mem.c"
                        INCLUDE_DIRS "."
                        EMBED_TXTFILES server_root_cert.pem
                        EMBED_FILES test_10.jpeg
                        REQUIRES nvs_flash protocol_examples_common mbedtls jpeg_turbo_codec)
