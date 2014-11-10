# Application condition
balance.id == max_used_id and not cur_node_is_processed

# Reaction
balance_init_code = "balance_init();\n"

code.append([balance_init_code])
id_to_pos_in_code[balancer.id] = len(code) - 1

balancer_code = "#include \"balancer.h\"\n"

cur_node_is_processed = True
