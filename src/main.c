#include "pico/stdlib.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#include "lcd.h"
#include "sleep.h"

lcd_t _lcd, *lcd;

static const uint8_t cc_duck[8] = { 0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0 };

#if 0
lcd = &_lcd;

  // ptr, address, width, height
  make_lcd(lcd, 0x27, 16, 2, LCD_SDA, LCD_SCL);

  lcd_make_custom_char(lcd, 0, cc_duck);
  lcd_clear(lcd);
  lcd_home(lcd);

  lcd_cursor_off(lcd);

  lcd_string(lcd, "Hello! ");
  lcd_custom_char(lcd, 0);
  lcd_set_cursor(lcd, 0, 1);
  lcd_printf(lcd, "lcd size: %li", sizeof(struct lcd_s));
#endif

struct lcd_char_s {
  uint8_t type  : 1;
  uint8_t value : 7;
};
typedef struct lcd_char_s lcd_char_t;

uint8_t lcd_cx = 0, lcd_cy = 0;
lcd_char_t lcd_buf0[LCD_MAX_CHARS] = { 0 };
lcd_char_t lcd_buf1[LCD_MAX_CHARS] = { 0 };

int main() {
	stdio_init_all();

	uint8_t sda_pin = PICO_DEFAULT_I2C_SDA_PIN;
	uint8_t scl_pin = PICO_DEFAULT_I2C_SCL_PIN;

	lcd = &_lcd;
	lcd_make(lcd, 0x27, 16, 2, sda_pin, scl_pin);

	uint8_t duck_id = 0;
	lcd_make_custom_char(lcd, duck_id, cc_duck);
	lcd_clear(lcd);
	lcd_home(lcd);
	lcd_cursor_off(lcd);

	const char * str1 = "Number: ";
	const size_t str1len = strlen(str1);

// 	lcd_string(lcd, str1);
//	lcd_set_cursor(lcd, 0, 1);
//	lcd_custom_char(lcd, duck_id);
	
	lcd_buf0[0] = (lcd_char_t){ .type = 0, .value = 'a' };
	lcd_buf0[1] = (lcd_char_t){ .type = 1, .value = 0 };
	lcd_buf0[2] = (lcd_char_t){ .type = 0, .value = '?' };
	int i = 0;
	while(i < LCD_MAX_CHARS - 1) {
		lcd_char_t c = lcd_buf0[i];
		if(c.type == 0 && c.value == 0) break;
		switch(c.type) {
			case 0:
				lcd_char(lcd, c.value);
			break;
			case 1:
				lcd_custom_char(lcd, c.value);
			break;
		};
		lcd_cx += 1;
		if(lcd_cx >= 16) {
			lcd_cx = 0;
			lcd_cy = (lcd_cy + 1) % 2;
			lcd_set_cursor(lcd, lcd_cx, lcd_cy);
		}
		i++;
	}
	sleep_ms(1000);

	int k = 0;
	while(1) {
		#if 0
		lcd_printf(lcd, "Number: %i", k);
		lcd_set_cursor(lcd, 0, 1);
		lcd_custom_char(lcd, duck_id);

		delay(500);

		lcd_home(lcd);
		lcd_clear(lcd);
		#endif
		
		// sleep_ns(2000);
		lcd_set_cursor(lcd, str1len, 0);
		lcd_printf(lcd, "%i", k);
		k++;
		// delay(50);
	}
	return 0;
}
