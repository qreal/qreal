while not (model.AnalogSensors[Trik.SensorPort.@@PORT@@].Read() @@SIGN@@ @@DISTANCE@@) do
    model.Sleep(10);
