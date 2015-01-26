while (!(brick.sensor(@@PORT@@).read() @@SIGN@@ @@DISTANCE@@)) {
	script.wait(10);
}
