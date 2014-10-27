# Apllication condition
end.id not in processed_ends

# Reaction
end_code = "TerminateTask();\n"

processed_ends.append(end.id)

code.append([end_code])
id_to_pos_in_code[end.id] = len(code) - 1

ifGeneration()

save()
