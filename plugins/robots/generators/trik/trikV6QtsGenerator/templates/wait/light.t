while (!(brick.sensor(@@PORT@@).read() @@SIGN@@ @@PERCENTS@@)) {
	script.wait(10);
}
