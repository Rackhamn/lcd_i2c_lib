#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define LCD_ADDR 0x27  // I2C address of the PCF8574 (adjust if needed)
#define LCD_RS   0     // Register Select pin (connected to the PCF8574)
#define LCD_RW   1     // Read/Write pin (connected to the PCF8574, usually grounded)
#define LCD_EN   2     // Enable pin (connected to the PCF8574)
#define LCD_D4   4     // Data pin D4 (4-bit mode, connected to the PCF8574)
#define LCD_D5   5     // Data pin D5
#define LCD_D6   6     // Data pin D6
#define LCD_D7   7     // Data pin D7

// Function to initialize I2C
void i2c_init_pico() {
    // Initialize I2C 0 at 400kHz
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C); // SDA pin
    gpio_set_function(5, GPIO_FUNC_I2C); // SCL pin
    gpio_pull_up(4);
    gpio_pull_up(5);
}

// Function to write a byte of data over I2C
void i2c_write(uint8_t data) {
    i2c_write_blocking(i2c0, LCD_ADDR, &data, 1, false);
}

// Function to pulse the Enable pin (E)
void lcd_strobe(uint8_t data) {
    i2c_write(data | (1 << LCD_EN));  // Set Enable high
    sleep_us(1);  // 1us delay
    i2c_write(data & ~(1 << LCD_EN)); // Set Enable low
    sleep_us(50); // 50us delay
}

// Function to send a command to the LCD
void lcd_write_cmd(uint8_t cmd) {
    uint8_t high_nibble = cmd & 0xF0;  // Get the high nibble
    uint8_t low_nibble = (cmd << 4) & 0xF0;  // Get the low nibble
    
    // Send the high nibble
    i2c_write(high_nibble);  // Send high nibble (RS = 0)
    lcd_strobe(high_nibble);
    
    // Send the low nibble
    i2c_write(low_nibble);  // Send low nibble (RS = 0)
    lcd_strobe(low_nibble);
    
    sleep_ms(40);  // Wait for the LCD to process the command
}

// Function to initialize the LCD in 4-bit mode
void lcd_init() {
    sleep_ms(15);  // Wait for 15ms after power-up

    // Initialize in 4-bit mode (3x 0x03, 1x 0x02)
    lcd_write_cmd(0x03);
    lcd_write_cmd(0x03);
    lcd_write_cmd(0x03);
    lcd_write_cmd(0x02);  // Set 4-bit mode
    
    // Display ON, Cursor OFF, Blink OFF
    lcd_write_cmd(0x0C);
    
    // Clear display
    lcd_write_cmd(0x01);
    
    // Entry Mode: Increment cursor, no display shift
    lcd_write_cmd(0x06);
}

// Function to write a string to the LCD
void lcd_putstr(const char *str) {
    while (*str) {
        uint8_t data = *str;
        i2c_write(data | (1 << LCD_RS));  // RS = 1 for data
        lcd_strobe(data);
        str++;
    }
}

int main() {
    // Initialize the Pico
    stdio_init_all();

    // Initialize I2C
    i2c_init_pico();

    // Initialize the LCD in 4-bit mode
    lcd_init();

    // Display a string on the LCD
    lcd_putstr("Hello, Pico LCD!");

    while (true) {
        tight_loop_contents();  // Keep the program running
    }

    return 0;
}

