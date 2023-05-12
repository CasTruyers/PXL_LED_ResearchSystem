idf_component_register(SRCS "SNTP.cpp" "nvs.cpp" "webserver.cpp" "softAP.cpp" "main.cpp" "LEDdriver.cpp" INCLUDE_DIRS "." )
spiffs_create_partition_image(storage ../data FLASH_IN_PROJECT)