while not (model.Sensor[@@PORT@@].Read() @@SIGN@@ @@DISTANCE@@) do
	model.Sleep(10);

