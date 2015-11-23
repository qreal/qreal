while not(model.encoder("@@PORT@@").read() @@SIGN@@ @@TACHO_LIMIT@@)) 
	Thread.Sleep 10
