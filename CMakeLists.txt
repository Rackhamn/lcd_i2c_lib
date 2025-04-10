cmake_minimum_required(VERSION 3.13)

# Set the PICO_BOARD variable
set(PICO_BOARD pico_w)

# Include the Pico SDK
include(pico_sdk_import.cmake)

# Define the project
project(test_project C CXX ASM)

# Set the C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Create the executable
add_executable(plcd src/main.c src/lcd.c src/sleep.c)
# src/scan_i2c.c)

# Enable USB and UART for the executable
pico_enable_stdio_usb(plcd 1)
pico_enable_stdio_uart(plcd 0)
# -> 0
# pico_enable_stdio_i2c(plcd 1)

# Add extra outputs (e.g., UF2 file)
pico_add_extra_outputs(plcd)
# add_compile_definitions(CYW43_LWIP)

# Link the necessary libraries
target_link_libraries(plcd 
  pico_stdlib 
  hardware_i2c 
  pico_cyw43_arch_none
)
	# pico_cyw43_arch_lwip_threadsafe_background)
