while (!(ecrobot_get_sound_sensor(@@PORT@@) * 100 / 1023 @@SIGN@@ @@VOLUME@@)) {
	wait(5);
}