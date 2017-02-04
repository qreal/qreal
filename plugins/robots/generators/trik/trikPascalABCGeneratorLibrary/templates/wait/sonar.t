while not (model.Sonars[Trik.SonarPort.@@PORT@@].Read() @@SIGN@@ @@DISTANCE@@) do
    model.Sleep(10);
