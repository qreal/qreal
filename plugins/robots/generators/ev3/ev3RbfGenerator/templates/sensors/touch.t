INPUT_READ(0, @@PORT@@, 1, 0, _temp_sensor_value)
CP_GT8(_temp_sensor_value, 0, _temp_sensor_value)
MOVE8_32(_temp_sensor_value, @@RESULT@@)