# Application condition
beep.id == max_used_id and not cur_node_is_processed

# Reaction
beep_code = "ecrobot_sound_tone(1000, 100, " + str(beep.Volume)+ ");\n"

code.append([beep_code])
id_to_pos_in_code[beep.id] = len(code) - 1

cur_node_is_processed = True
