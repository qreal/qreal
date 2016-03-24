while not (model.encoder("@@PORT@@").Read() @@SIGN@@ @@TACHO_LIMIT@@))
    model.Sleep(10);
