while model.AnalogSensors[Trik.SensorPort.@@PORT@@].Read() <> 1 do
    model.Sleep(10);
