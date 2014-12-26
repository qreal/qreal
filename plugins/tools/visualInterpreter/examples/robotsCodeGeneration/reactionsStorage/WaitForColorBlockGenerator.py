# Application condition
waitFor.id == max_used_id and not cur_node_is_processed

# Reaction
port = "NXT_PORT_S" + waitFor.Port

color_nxt_type = ""
color_str = waitFor.Color

if color_str == "Красный":
  color_nxt_type = "NXT_COLOR_RED"
elif color_str == "Зелёный":
  color_nxt_type = "NXT_COLOR_GREEN"
elif color_str == "Синий":
  color_nxt_type = "NXT_COLOR_BLUE"
elif color_str == "Чёрный":
  color_nxt_type = "NXT_COLOR_BLACK"
elif color_str == "Жёлтый":
  color_nxt_type = "NXT_COLOR_YELLOW"
elif color_str == "Белый":
  color_nxt_type = "NXT_COLOR_WHITE"

wait_for_color_block_code = "while (ecrobot_get_nxtcolorsensor_id(" + port + ") != " + color_nxt_type + ") {}\n"
wait_init_code = "ecrobot_init_nxtcolorsensor(" + port + ", " + color_nxt_type + ");\n"
wait_terminate_code = "ecrobot_init_nxtcolorsensor(" + port + ", " + color_nxt_type + ");\n"

if wait_init_code not in init_code:
  init_code.append(wait_init_code)
  terminate_code.append(wait_terminate_code)

code.append([wait_for_color_block_code])
id_to_pos_in_code[waitFor.id] = len(code) - 1

cur_node_is_processed = True
