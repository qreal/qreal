# Application condition
timer.id == max_used_id and not cur_node_is_processed

# Reaction
timer_code = "systick_wait_ms(" + timer.Delay + ");\n"

code.append([timer_code])
id_to_pos_in_code[timer.id] = len(code) - 1

cur_node_is_processed = True
