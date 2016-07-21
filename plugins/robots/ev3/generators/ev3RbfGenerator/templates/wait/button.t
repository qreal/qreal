@@RANDOM_ID@@:
TIMER_WAIT(10, timer)
TIMER_READY(timer)
UI_BUTTON(PRESSED, @@BUTTON@@, _temp_sensor_value_8)
JR_FALSE(_temp_sensor_value_8, @@RANDOM_ID@@)
