DATA32 temp_power_motor_value_42
MOVE32_32(@@POWER@@, temp_power_motor_value_42)
MUL32(temp_power_motor_value_42, -1, temp_power_motor_value_42)
OUTPUT_POWER(0, @@PORT@@, temp_power_motor_value_42)
OUTPUT_START(0, @@PORT@@)
