while (!(brick.sensor("JA@@PORT@@").read() @@SIGN@@ @@PERCENTS@@)) {
	brick.wait(10);
}
