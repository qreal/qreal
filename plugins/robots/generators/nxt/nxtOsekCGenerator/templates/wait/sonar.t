while (!(ecrobot_get_sonar_sensor(@@PORT@@) @@SIGN@@ @@DISTANCE@@)) {
	wait(50);
}
