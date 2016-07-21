@@RANDOM_ID@@:
TIMER_WAIT(10, timer)
TIMER_READY(timer)
INPUT_READ(0, @@PORT@@, 0, 0, _temp_sensor_value_8)
JR_EQ8(_temp_sensor_value_8, 0, @@RANDOM_ID@@)
