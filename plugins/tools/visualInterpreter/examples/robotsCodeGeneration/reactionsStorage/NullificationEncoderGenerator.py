# Application condition
encoder.id == max_used_id and not cur_node_is_processed

# Reaction
nullification_code = "nxt_motor_set_count(NXT_PORT_" + encoder.Port + ", 0);\n"

code.append([nullification_code])
id_to_pos_in_code[encoder.id] = len(code) - 1

cur_node_is_processed = True
