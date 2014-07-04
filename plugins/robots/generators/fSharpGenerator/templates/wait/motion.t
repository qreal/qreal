while not(model.AnalogSensor.["F1"].Read() = 0) do
	Thread.Sleep 10

