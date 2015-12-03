INPUT_READ(0, @@PORT@@, 1, 0, _temp_sensor_value_8)
CP_GT8(_temp_sensor_value_8, 0, _temp_sensor_value_8)
MOVE8_32(_temp_sensor_value_8, @@RESULT@@)