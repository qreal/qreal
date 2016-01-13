while (!(ecrobot_get_nxtcolorsensor_light(@@PORT@@) * 100 / 1023 @@SIGN@@ @@INTENSITY@@)) {
	wait(5);
}