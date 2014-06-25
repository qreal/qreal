while (brick.sensor("F1").read() == 0) {
	brick.wait(10);
}
