runCounter = 0;
runWasDown = 0;
while (runCounter < @@RUN_CLICKS@@) {
	if (!ecrobot_is_RUN_button_pressed() && runWasDown) {
		++runCounter;
		runWasDown = 0;
	}
	if (ecrobot_is_RUN_button_pressed()) {
		runWasDown = 1;
	}
}
