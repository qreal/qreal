while (!(brick.sensor("@@PORT@@").read() @@SIGN@@ @@PERCENTS@@)) {
	brick.wait(10);
}
