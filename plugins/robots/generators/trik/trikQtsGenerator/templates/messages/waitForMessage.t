while (!mailbox.hasMessages()) {
	script.wait(100);
}
@@VARIABLE@@ = mailbox.receive();
