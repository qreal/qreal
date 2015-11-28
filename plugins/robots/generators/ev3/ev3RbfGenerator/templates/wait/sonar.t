while (!(ecrobot_get_sonar_sensor(@@PORT@@) @@SIGN@@ @@DISTANCE@@)) {
	systick_wait_ms(50);
}
