@@RANDOM_ID@@:
TIMER_WAIT(10, timer)
TIMER_READY(timer)
INPUT_READSI(0, @@PORT@@, 0, 2, _temp_sensor_value_f)
MOVEF_8(_temp_sensor_value_f, _temp_sensor_value_8)
JR_NEQ8(_temp_sensor_value_8, @@COLOR@@, @@RANDOM_ID@@)
