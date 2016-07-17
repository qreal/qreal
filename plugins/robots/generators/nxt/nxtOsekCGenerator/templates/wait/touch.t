while (!ecrobot_get_touch_sensor(@@PORT@@)) {
	wait(5);
}