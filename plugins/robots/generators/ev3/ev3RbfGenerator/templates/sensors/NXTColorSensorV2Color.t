// return data in format DATA32 C 0-17
ARRAY8 ar@@RANDOM_ID_1@@ 4
ARRAY(CREATE8, 4, ar@@RANDOM_ID_1@@)

//enables lights
ARRAY_WRITE(ar@@RANDOM_ID_1@@, 0, 1)
ARRAY_WRITE(ar@@RANDOM_ID_1@@, 1, 65)
ARRAY_WRITE(ar@@RANDOM_ID_1@@, 2, 0)
INPUT_DEVICE(SETUP, 0, @@PORT@@, 1, 0, 3, @ar@@RANDOM_ID_1@@, 0, @ar@@RANDOM_ID_1@@)
TIMER_WAIT(110, timer)
TIMER_READY(timer)

//read color
ARRAY_WRITE(ar@@RANDOM_ID_1@@, 0, 1)
ARRAY_WRITE(ar@@RANDOM_ID_1@@, 1, 66)
ARRAY8 answer@@RANDOM_ID_2@@ 4
ARRAY(CREATE8, 4, answer@@RANDOM_ID_2@@)
INPUT_DEVICE(SETUP, 0, @@PORT@@, 1, 0, 2, @ar@@RANDOM_ID_1@@, 1, @answer@@RANDOM_ID_2@@)
ARRAY_READ(answer@@RANDOM_ID_2@@, 0, _temp_sensor_value_8)
ARRAY(DELETE, answer@@RANDOM_ID_2@@)
ARRAY(DELETE, ar@@RANDOM_ID_1@@)
MOVE8_32(_temp_sensor_value_8, @@RESULT@@)