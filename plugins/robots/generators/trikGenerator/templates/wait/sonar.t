while (!(brick.sensor("JA@@PORT@@").read() @@SIGN@@ @@DISTANCE@@)) {
	brick.wait(10);
}
