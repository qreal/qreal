while (brick.sensor("F1").read() != 1) {
	brick.wait(10);
}
