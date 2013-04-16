# Application condition
playTone.id == max_used_id and not cur_node_is_processed

# Reaction
play_tone_code = "ecrobot_sound_tone(" + playTone.Frequency + ", " + playTone.Duration + ", " + playTone.Volume + ");\n"

code.append([play_tone_code])
id_to_pos_in_code[playTone.id] = len(code) - 1

cur_node_is_processed = True
