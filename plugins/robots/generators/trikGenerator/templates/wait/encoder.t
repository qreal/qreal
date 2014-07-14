while (!(brick.encoder("@@PORT@@").read() @@SIGN@@ @@TACHO_LIMIT@@)) {
	brick.wait(10);
}