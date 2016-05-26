while not (model.Encoders[Trik.EncoderPort.@@PORT@@].Read() @@SIGN@@ @@TACHO_LIMIT@@) do
    model.Sleep(10);
