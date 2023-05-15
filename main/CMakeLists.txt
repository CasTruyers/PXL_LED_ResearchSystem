idf_component_register(SRCS "wifi.cpp" "time.cpp" "nvs.cpp" "webserver.cpp" "main.cpp" "LEDdriver.cpp" INCLUDE_DIRS "." )
spiffs_create_partition_image(storage ../data FLASH_IN_PROJECT)