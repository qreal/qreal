while (!(brick.sensor("JF1").read() @@SIGN@@ @@DISTANCE@@)) {
	brick.wait(10);
}
