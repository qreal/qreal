for (int _counter = 0; _counter < sizeof(lcd); ++_counter) {
	lcd_copy[_counter] = lcd[_counter];
}
ecrobot_bmp2lcd(BMP_DATA_START(@@IMAGE_NAME@@), lcd, 100, 64);
for (int _counter = 0; _counter < sizeof(lcd); ++_counter) {
	lcd[_counter] |= lcd_copy[_counter];
}
display_bitmap_copy(lcd, 100, 8, 0, 0);
display_update();