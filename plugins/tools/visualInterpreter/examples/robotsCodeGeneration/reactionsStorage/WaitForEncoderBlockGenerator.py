# Application condition
waitFor.id == max_used_id and not cur_node_is_processed

# Reaction
wait_for_encoder_code = "while (nxt_motor_get_count(NXT_PORT_" + waitFor.Port + ") < " + waitFor.TachoLimit + ") {}\n"

code.append([wait_for_encoder_code])
id_to_pos_in_code[waitFor.id] = len(code) - 1

cur_node_is_processed = True
