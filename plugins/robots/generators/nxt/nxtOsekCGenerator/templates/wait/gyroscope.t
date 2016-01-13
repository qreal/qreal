while (!(ecrobot_get_gyro_sensor(@@PORT@@) @@SIGN@@ @@DEGREES@@)) {
	wait(5);
}
