while (!brick.mailbox().hasMessages()) {
	brick.wait(100);
}
@@VARIABLE@@ = brick.mailbox().receive();
