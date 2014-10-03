while (!(brick.sensor(@@PORT@@).read() @@SIGN@@ @@DISTANCE@@)) {
	brick.wait(10);
}
