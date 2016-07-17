while not (model.AnalogSensors[Trik.SensorPort.@@PORT@@].Read() * 100 / 1024 @@SIGN@@ @@PERCENTS@@) do
    model.Sleep(10);
