while (!(ecrobot_get_sonar_sensor(@@PORT@@) @@CONDITION@@ @@DISTANCE@@)) {
	systick_wait_ms(10);
}
