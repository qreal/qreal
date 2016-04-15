while model.AnalogSensors[Trik.SensorPort.@@PORT@@].Read() < 1024 / 2 do
    model.Sleep(10);
