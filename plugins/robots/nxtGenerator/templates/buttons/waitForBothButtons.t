enterCounter = 0;
enterWasDown = 0;
runCounter = 0;
runWasDown = 0;
while (enterCounter < @@ENTER_CLICKS@@ || runCounter < @@RUN_CLICKS@@) {
	if (!ecrobot_is_ENTER_button_pressed() && enterWasDown) {
		++enterCounter;
		enterWasDown = 0;
	}
	if (ecrobot_is_ENTER_button_pressed()) {
		enterWasDown = 1;
	}
	if (!ecrobot_is_RUN_button_pressed() && runWasDown) {
		++runCounter;
		runWasDown = 0;
	}
	if (ecrobot_is_RUN_button_pressed()) {
		runWasDown = 1;
	}
}
