# Application condition
engine.id == max_used_id and not cur_node_is_processed

# Reaction
power = engine.Power
brake_mode = "1"
ports = engine.Ports
port_names = []

collectPortNames(ports, port_names)

sign_rotate = "-"
init_terminate_velocity = "0"
init_terminate_brake_mode = "1"
engines_backward_code = ""

for port in port_names:
  code.append(["nxt_motor_set_speed(" + port + ", " + sign_rotate + power + ", " + brake_mode + ");\n"])
  if engine.id not in id_to_pos_in_code:
    id_to_pos_in_code[engine.id] = len(code) - 1
  init_code_engines = "nxt_motor_set_speed(" + port + ", " + init_terminate_velocity + ", " + init_terminate_brake_mode + ");\n"
  terminate_code_engines = "nxt_motor_set_speed(" + port + ", " + init_terminate_velocity + ", " + init_terminate_brake_mode + ");\n"
  if init_code_engines not in init_code:
    init_code.append(init_code_engines)
    terminate_code.append(terminate_code_engines)

cur_node_is_processed = True
