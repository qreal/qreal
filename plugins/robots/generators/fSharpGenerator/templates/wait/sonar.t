while not(model.AnalogSensor.["@@PORT@@"].Read() @@SIGN@@ @@DISTANCE@@) do
	Thread.Sleep 10

