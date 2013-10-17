enterCounter = 0;
enterWasDown = 0;
while (enterCounter < @@ENTER_CLICKS@@) {
	if (!ecrobot_is_ENTER_button_pressed() && enterWasDown) {
		++enterCounter;
		enterWasDown = 0;
	}
	if (ecrobot_is_ENTER_button_pressed()) {
		enterWasDown = 1;
	}
}
