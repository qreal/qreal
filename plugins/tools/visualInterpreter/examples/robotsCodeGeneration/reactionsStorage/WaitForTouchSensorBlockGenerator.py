# Application condition
waitFor.id == max_used_id and not cur_node_is_processed

# Reaction
wait_for_touch_sensor_code = "while (!ecrobot_get_touch_sensor(NXT_PORT_S" + waitFor.Port + ")) {}\n"

code.append([wait_for_touch_sensor_code])
id_to_pos_in_code[waitFor.id] = len(code) - 1

cur_node_is_processed = True
