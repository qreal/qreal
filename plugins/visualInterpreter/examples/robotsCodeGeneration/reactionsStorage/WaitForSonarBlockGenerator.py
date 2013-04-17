# Application condition
waitFor.id == max_used_id and not cur_node_is_processed

# Reaction
port = "NXT_PORT_S" + waitFor.Port
condition = convertCondition(waitFor.Sign) + " " + waitFor.Distance

wait_for_sonar_code = "while (!(ecrobot_get_sonar_sensor(" + port + ") " + condition + ")) {}\n"
wait_init_code = "ecrobot_init_sonar_sensor(" + port + ");\n"
wait_terminate_code = "ecrobot_term_sonar_sensor(" + port + ");\n"

if wait_init_code not in init_code:
  init_code.append(wait_init_code)
  terminate_code.append(wait_terminate_code)

code.append([wait_for_sonar_code])
id_to_pos_in_code[waitFor.id] = len(code) - 1

cur_node_is_processed = True
