cmake_minimum_required(VERSION 2.8.11)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set (CMAKE_C_FLAGS "--std=c99 -g ${CMAKE_C_FLAGS}")
endif()

file(GLOB cloud_c_files "cloud/*.c")
file(GLOB cloud_h_files "cloud/*.h")

file(GLOB device_c_files "device/*.c")
file(GLOB device_h_files "device/*.h")

file(GLOB payload_c_files "payload/*.c")
file(GLOB payload_h_files "payload/*.h")

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
  include_directories(
    "."
    "./cloud"
    "./device"
    "./payload"
    "/usr/include/.")
else()
  include_directories(
    "./cloud"
    "./device"
    "./payload"
    "/usr/include/azureiot"
    "/usr/include/azureiot/inc/")
endif()

add_executable(
    iothub_client_sample_weatherStation
    main.c
    ${cloud_c_files}
    ${cloud_h_files}
    ${device_c_files}
    ${device_h_files}
    ${payload_c_files}
    ${payload_h_files})

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
  target_link_libraries(
    iothub_client_sample_weatherStation
    wiringPi
    serializer
    iothub_client
    iothub_client_mqtt_transport
    umqtt
    aziotsharedutil
    ssl
    crypto
    curl
    pthread
    m)
else()
  target_link_libraries(
    iothub_client_sample_weatherStation
    serializer
    iothub_client
    iothub_client_mqtt_transport
    umqtt
    aziotsharedutil
    ssl
    crypto
    curl
    pthread
    m)
endif()