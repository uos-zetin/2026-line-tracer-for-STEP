target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ./Inc)
file(GLOB SOURCES ./Src/*.c)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES})