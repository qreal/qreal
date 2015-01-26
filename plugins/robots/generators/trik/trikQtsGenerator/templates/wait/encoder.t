while (!(brick.encoder(@@PORT@@).read() @@SIGN@@ @@TACHO_LIMIT@@)) {
	script.wait(10);
}
