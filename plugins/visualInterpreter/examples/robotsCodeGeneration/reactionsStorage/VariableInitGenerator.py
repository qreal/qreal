# Application condition
var.id == max_used_id and not cur_node_is_processed

# Reaction
var_code = var.variable + " = " + var.value + ";\n"

code.append([var_code])
id_to_pos_in_code[var.id] = len(code) - 1

cur_node_is_processed = True
