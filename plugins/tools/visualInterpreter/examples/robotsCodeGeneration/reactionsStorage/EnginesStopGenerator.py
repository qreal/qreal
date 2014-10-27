# Application condition
engine.id == max_used_id and not cur_node_is_processed

# Reaction
ports = engine.Ports
port_names = []

collectPortNames(ports, port_names)

engines_stop_code = ""

for port in port_names:
  code.append(["nxt_motor_set_speed(" + port + ", 0, 1);\n"])
  if engine.id not in id_to_pos_in_code:
    id_to_pos_in_code[engine.id] = len(code) - 1

cur_node_is_processed = True
