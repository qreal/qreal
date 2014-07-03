while not(model.AnalogSensor.["F1"].Read() @@SIGN@@ @@DISTANCE@@) do
	Thread.Sleep 10

