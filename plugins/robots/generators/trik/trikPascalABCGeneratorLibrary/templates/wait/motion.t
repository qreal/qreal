while not (robot.Sensor["F1"].Read() = 0) do
    model.Sleep(10)
