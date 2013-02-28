# Application condition
balance.id == max_used_id and not cur_node_is_processed

# Reaction
balance_code = "balance_control((F32)" + balancer.forward + ", (F32)" + balancer.turn + ", (F32)ecrobot_get_gyro_sensor(NXT_PORT_S" + balancer.port + "), (F32)" + balancer.gyroOffset + ", (F32)nxt_motor_get_count(NXT_PORT_" + balancer.port1 + "), (F32)nxt_motor_get_count(NXT_PORT_" + balancer.port2 + "), (F32)ecrobot_get_battery_voltage(), &" + balancer.pwm1 + ", &" + balancer.pwm2 + ");\n"

code.append([balance_code])
id_to_pos_in_code[balancer.id] = len(code) - 1

cur_node_is_processed = True
