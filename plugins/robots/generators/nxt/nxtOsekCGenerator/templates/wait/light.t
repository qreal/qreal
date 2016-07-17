while (!(100 - ecrobot_get_light_sensor(@@PORT@@) * 100 / 1023 @@SIGN@@ @@PERCENTS@@)) {
	wait(10);
}
