cmake_minimum_required(VERSION 3.16)
project(PhilipsHueWithArduino)
add_executable(philips_hue_controller src/main.cpp src/Lights.cpp src/CurlClient.cpp)

# Link libcurl
target_link_libraries(philips_hue_controller curl)

# Find fmt
target_link_libraries(philips_hue_controller fmt)