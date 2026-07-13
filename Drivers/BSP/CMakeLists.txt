target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ./ST7735 ./ST7789)
file(GLOB SOURCES ./ST7735/*.c ./ST7789/*.c)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES})